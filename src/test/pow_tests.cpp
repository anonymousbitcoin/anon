// Copyright (c) 2015 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"
#include "pow.h"
#include "util.h"
#include "test/test_bitcoin.h"

#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_FIXTURE_TEST_SUITE(pow_tests, BasicTestingSetup)

/* Test calculation of next difficulty target with no constraints applying */
BOOST_AUTO_TEST_CASE(get_next_work)
{
    SelectParams(CBaseChainParams::MAIN);
    const Consensus::Params& params = Params().GetConsensus();

    int64_t nLastRetargetTime = 1262149169; // NOTE: Not an actual block time
    int64_t nThisTime = 1262152739;  // Block #32255 of Bitcoin
    arith_uint256 bnAvg;
    bnAvg.SetCompact(0x1d014f3c);
    BOOST_CHECK_EQUAL(0x1d011998,
                      DigishieldCalculateNextWorkRequired(bnAvg, nThisTime, nLastRetargetTime, params, UintToArith256(params.powLimit)));
}

/* Test the constraint on the lower bound for actual time taken */
BOOST_AUTO_TEST_CASE(get_next_work_lower_limit_actual)
{
    SelectParams(CBaseChainParams::MAIN);
    const Consensus::Params& params = Params().GetConsensus();

    int64_t nLastRetargetTime = 1279296753; // NOTE: Not an actual block time
    int64_t nThisTime = 1279297671;  // Block #68543 of Bitcoin
    arith_uint256 bnAvg;
    bnAvg.SetCompact(0x1c05a3f4);
    BOOST_CHECK_EQUAL(0x1c04bceb,
                      DigishieldCalculateNextWorkRequired(bnAvg, nThisTime, nLastRetargetTime, params, UintToArith256(params.powLimit)));
}

/* Test the constraint on the upper bound for actual time taken */
BOOST_AUTO_TEST_CASE(get_next_work_upper_limit_actual)
{
    SelectParams(CBaseChainParams::MAIN);
    const Consensus::Params& params = Params().GetConsensus();

    int64_t nLastRetargetTime = 1269205629; // NOTE: Not an actual block time
    int64_t nThisTime = 1269211443;  // Block #46367 of Bitcoin
    arith_uint256 bnAvg;
    bnAvg.SetCompact(0x1C58C841);
    BOOST_CHECK_EQUAL(0x1c4a93bb,
                      DigishieldCalculateNextWorkRequired(bnAvg, nThisTime, nLastRetargetTime, params, UintToArith256(params.powLimit)));
}

BOOST_AUTO_TEST_CASE(GetBlockProofEquivalentTime_test)
{
    SelectParams(CBaseChainParams::MAIN);
    const Consensus::Params& params = Params().GetConsensus();

    std::vector<CBlockIndex> blocks(10000);
    for (int i = 0; i < 10000; i++) {
        blocks[i].pprev = i ? &blocks[i - 1] : NULL;
        blocks[i].nHeight = i;
        if (blocks[i].nHeight < Params().GetConsensus().vUpgrades[Consensus::UPGRADE_ECHELON].nActivationHeight) {
            blocks[i].nTime = 1269211443 + i * params.nPowTargetSpacing;
        }
        else {
            blocks[i].nTime = 1269211443 + i * params.nPowTargetSpacingEchelon;
        }
        blocks[i].nBits = 0x207fffff; /* target 0x7fffff000... */
        blocks[i].nChainWork = i ? blocks[i - 1].nChainWork + GetBlockProof(blocks[i - 1]) : arith_uint256(0);
    }

    for (int j = 0; j < 1000; j++) {
        CBlockIndex *p1 = &blocks[GetRand(10000)];
        CBlockIndex *p2 = &blocks[GetRand(10000)];
        CBlockIndex *p3 = &blocks[GetRand(10000)];

        int64_t tdiff = GetBlockProofEquivalentTime(*p1, *p2, *p3, params);
        BOOST_CHECK_EQUAL(tdiff, p1->GetBlockTime() - p2->GetBlockTime());
    }
}

BOOST_AUTO_TEST_SUITE_END()
