// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"
#include "crypto/equihash.h"
#include "util.h"
#include "utilstrencodings.h"
#include <assert.h>
#include <boost/assign/list_of.hpp>
#include "base58.h"
using namespace std;
#include "chainparamsseeds.h"

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        strCurrencyUnits = "ANON";
        consensus.fCoinbaseMustBeProtected = true;

        //TODO To be decided
        // consensus.nSubsidySlowStartInterval = 2;
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 4000;
        consensus.prePowLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // consensus.powLimit = uint256S("07ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 2016; //diffuculty adjusts every 2 weeks
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 32; // 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // 16% adjustment up
        consensus.nPowTargetSpacing = 10 * 60; // time between blocks (sec)
        consensus.fPowAllowMinDifficultyBlocks = false;
        
        //TODO To be decided 
        // consensus.nPowDifficultyBombHeight = 600000;

        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        pchMessageStart[0] = 0x83;
        pchMessageStart[1] = 0xd8;
        pchMessageStart[2] = 0x47;
        pchMessageStart[3] = 0xa7;
        vAlertPubKey = ParseHex("04f2cd746e629ffd320a81287474c98c2ad15d15b0a210b0144edcd8f3e1301c6311fd751fa34ba17d88090374cfec7cd9aaca55a5a0c4456511acc01b922005de");
        nDefaultPort = 33130;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 100000;
        const size_t N = 144, K = 5;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

        /**
         * Build the genesis block. Note that the output of its generation
         * transaction cannot be spent since it did not originally exist in the
         * database (and is in any case of zero value).
         *
         * >>> from pyblake2 import blake2s
         * >>> 'Zclassic' + blake2s(b'No taxation without representation. ANON #437541 - 00000000000000000397f175a94dd3f530b957182eb2a9f7b79a44a94a5e0450').hexdigest()
         */
        const char* pszTimestamp = "Zclassic860413afe207aa173afee4fcfa9166dc745651c754a41ea8f155646f5aa828ac";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 4;
        genesis.nTime    = 1528828158;
        genesis.nBits    = 0x1f07ffff;
        genesis.nNonce   = uint256S("0x0000000000000000000000000000000000000000000000000000000000000404");
        genesis.nSolution = ParseHex("001ad300894fa0c7f210c2ad79ccac3d5d4f58a36708ae5b158bec462b851c3282209edd2db556ddbf750bab1c6dfa8512075aac10bbd77dc2e8fe7b2d637b16029c92d1c5d2ef27d6a45e05d061b94f88eee4bf00caf182264f9bbf7b3502725752bef218e7f28497094e9f8e79e29713151527e177ec3486f8721e458618e6592d45933ed6e7f4b82100ca94e2d77956d9e92d6b6b71e51d5ffda3f13625afe0244a334afe0566099c1d4a6e120a85fa6fe7827fff09af6627fb86df21e574cebe183e55e471b5265cee9a3ad18ddabee914db6e65c4af87eb973fe7253aced3fbdeee9f79d928e03f18da6ae441582c0a07dbd559e682217f92f411f0a3b5754e05d12c1343facc52c019bbc93c30bd31690aedbd2026cdfc6cd74eb3e70e13520afea1321676cb6441560c72bf5d32d6a13fc330b76a98449623d39f36b1936bef51d0e288aaf59ada70fd197df4011a9d11509e7cdd27d9c3de006a591627f4d7ec0b0f73d1a2d6cb32b78bcdd3479adae5da049c1ffcd501f9ef92ea863d96a7129166c9646392d4e4df07e528525b4842e91eebfd7f15fd7773f166d56c3856b706a5a478af4308d8552f81264074fa613e52561b1449512e55891c7cdda037552d64f91d6a15b517ebf125b083aad5a4a967605c22b7fc2e606713d4bca11c460c12383752d1e12375f5654c51fb667e763f8f2d07ea0252a030f8e78caa7084c5bc996170e35d93a6155c3c1b33c89fa9616d11e924361e65d7975a09520ad5832e85af473d92bae0b00520ce36f91efe8f6a151d86a66894bce0fcb6538309d71a42405dbcddaf0a7ce8f869e34ed39370a114c9d761144730b38a1838210d3fd2e13c23beac7442feb0e6d61f8172baf11b966fb8f88ecc64829d6271f37eb538aa280e1ff326c72f5a90ccb25b8d4fa28d33634df6c1a71a5590009295632b6b95d56ade05facc59b005d391f25d5e0d7d7f23bf9ad7d7f7b1993ccbfcaac6a1289a68b5028d7390b9696fb778f3415de2445acab1555b80d60977d1159bc872cd5d051100b0380aeca5600bcd8901ac4d488c00caf5e46ac2124b62aefe930d157a570fa58c98b04c088befbc71dd3051d41acf463f0a05026b3e0e7dc44eef83920451002b69a28eec7cb25710bdfd89e72ad4af9718320156ce1845831274c4830a1491aea485a4fc8f8d129006b6e24ecc8c9ce2c31896922ba5cd05cd482f71a2be95cf7cf05479a3fc116a8c03ae09b200bd86833c77c04e652c457a13c62ccd6aa29c5c5ac3dd2223a86832027cfc9f3644950fe00c942539c455e343d84ae371c36e2d6af7c2e053b31ba4aa2bcf4ff32957e49ff5b2924692daeb8614cae2584e49a9e330bca859927d239f45af7ea43b2d2eda4e08d2f0c4d42d39c3424f0f5290aa3daead0105b83c124114d5c013f43474c26fed9a355f40b515fcf2a0cfcb2207f38de1e44025da829e02faed2108ef2ce7258a03ee769be95f98e6f2876f85dc682e2bfda3711be0d783478a5404f72c1b9299f9551faf0713c45f98955521c22ef399746ca80243c5b8c82d190af20218c93ba3b3862279fb9bd3fbf880fffa9b36af26c97d991320d5b1576460f6ca36c9685ff4623cdf3c784a35250dc25cb4ef4379911dcacdb40a610ca2670a7acd65352a6fe17bf05b4075e753588e560e12c6a1acd277774c4b64eafdfed697aa7cde517e1b6c554d80df30cbe18c11f33ded7d42f14dddefa02c149e516624c601509c66fb58c7c88ac6165d85d11defd13ef7159e8cdfb413031a402f591e69fd5665367133f120133db16158072ecfbd91f9cc65923a754bc3dafbd8704c51a5e0d5826c5dc12a2943def03b7eab5744b7ad3eb573f828dce2707d0a764c784ec0");

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0002aee7e57553028a6156fe28f6a74bfacc0a8c0cd45f19027b6adc0a628044"));
        assert(genesis.hashMerkleRoot == uint256S("0x19612bcf00ea7611d315d7f43554fa983c6e8c30cba17e52c679e0e80abf7d42"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // TODO: setup a DNSSeed

        //vSeeds.push_back(CDNSSeedData("[INSERT].org", "dnsseed.[INSERT].org"));
        //vSeeds.push_back(CDNSSeedData("[INSERT].co", "dnsseed.[INSERT].co"));
        
        // guarantees the first 2 characters, when base58 encoded, are "b1"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x13,0x25};
        // guarantees the first 2 characters, when base58 encoded, are "bx"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x13,0xAF};
        // the first character, when base58 encoded, is "5" or "K" or "L" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0x80};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x88,0xB2,0x1E};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x88,0xAD,0xE4};
        // guarantees the first 2 characters, when base58 encoded, are "zk"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xA8};
        // guarantees the first 2 characters, when base58 encoded, are "SK"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAB,0x36};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = {
            {
                {      0, consensus.hashGenesisBlock },
            },
            1520105951,     // * UNIX timestamp of last checkpoint block
            0,       // * total number of transactions between genesis and last checkpoint
                            //   (the tx=... number in the SetBestChain debug.log lines)
            2000            // * estimated number of transactions per day after checkpoint
                            //   total number of tx / (checkpoint block height / (24 * 24))
        };

        nForkStartHeight = 5;
        nForkHeightRange = 1000;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        strCurrencyUnits = "ANONT";
        consensus.fCoinbaseMustBeProtected = true;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 400;
        // consensus.powLimit = uint256S("07ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.prePowLimit = consensus.powLimit;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.fPowAllowMinDifficultyBlocks = true;

        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        consensus.nPowDifficultyBombHeight = 600000;

        pchMessageStart[0] = 0x7a;
        pchMessageStart[1] = 0x74;
        pchMessageStart[2] = 0x8d;
        pchMessageStart[3] = 0x38;

        vAlertPubKey = ParseHex("048679fb891b15d0cada9692047fd0ae26ad8bfb83fabddbb50334ee5bc0683294deb410be20513c5af6e7b9cec717ade82b27080ee6ef9a245c36a795ab044bb3");
        nDefaultPort = 33127;
        nPruneAfterHeight = 1000;


        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1479443947;
        genesis.nBits = 0x2007ffff;
        genesis.nNonce = uint256S("0x0000000000000000000000000000000000000000000000000000000000000005");
        genesis.nSolution = ParseHex("005e3ac191b3a989ef7be400ec5b5fea2ebadbd0261158d70dc94c979b6b705148658cd598575c1942b727546405b26c9497f72a4840d04d929e218819ce55454152705d11e59590cc97aee7f5df83112cbe99df0424864ac9542e4d381dd236b7736b6d89aaed43661614b422ded28fafa419038c1bd0f52db513fe70ae11d069da2d1f7c5f4bc076050fff481f1c381a6e5d2f3b763e46e4a8fd29da954d7a4564da4ef2dbe62100bf1b72e1dea5e5f884f44f61c60c2e63d233ea48024c69cbde427fb477a732aad2f6e4c4b287de8d4c0294fe2518d9c95393f3d3d52f7410bd06a31a1e8a1e31c12505aa7437a2afd2030c784892e6ff1a32eb06a20c2e5ad8c793b2c7e194067699893167129fb92c297df20b8edd0576a9934ed1309eb60332f5176d2598b6364c59225579a597f3f6d5877a50bef8634d2fdc33fbb796497b73d8cafe86fad7d74175dd997e06fcd64ca2cb4657f5d2644c637454ce5cba3641c07532756cab64bc29da22b7a754ded97e26a7b527ff0de47889d38e6be930f3f55c06ced1dd9e4e5c20981d7b7e8d98127ab3ac1d537030e170b71ad7bac6760927f581714d87fbd5625397b7715d6e63ca53a083210acb1e54944601d9db5332f629c4b96fee588e970ab15faa8d0bcd67328b81d94e722f1567121f8bac1097fca452e474c9728532aa9c292eae1be3534d64072bebfa87889feaeac851a2716ccf0569625d9a0c2bdb5cafd763153798a8f6d9eb42380a8d76dee1bc1c0e294bb9de92313bb6e62ec433c9761ecc77b03847403faca814ff58d8c7262914e2a35f05fa9f93070996a7ef6dcb633d0d9e8217446a9084b17a6ac3a711f863b80dc91d3b80bfd331c3e543fe8be839aa611302d4be02353149e2bb32c375b95de22ca05e857b18a1042d935a1387def7820b5c344b2ea864578a7d0316814b5383d2bd44b79161c5108a753aea33d5c848423d3c0bb3c7bda3015a3bfd72ac2b96a39ff6de0e4913166d201119491637eb60e49bc30103387f7413216f639f53fca6d0fdd2f56aa3f2769eeb76014604bb1069738167684e597218e9df144bdbf4bfd4631aabea0c139916bd3961526a6dfa31f8cfb3314e7a0ae9cd34e9124b396e9bd3f93be4a7f28d4dddab541702c183189135fbacbf722fec51af06709bb44e1408e0bcc1c2ef302fa561749ba03dd5ae500b1352b60e6ae5901965412be4869286071e4a2d1fa329c6350fd7975314dd82e33fe733bd3b6667e5b4d7fabe9218e6a74be946d60b9c08e4e37a3332f58dbdd369c6124891f869c657673046a5b5cb59cc49b5e416b6c265fde64feca5ec19a15d66fe0a7f71f1c4cbf41c7423150e7ca8c8f76f0c7bd4901ccac9bf8cef5d618224094c35d035dec3e5c168f198e8354ea5b5def4ef06baaf3d9e601699c97a12cbc64242a1fdbd192b012994edca779f48b7825144d19e517d9d3765be7b770d89cc5a9c754fc3ff9562bd0fb6ddcced8d098d8c279fcd7dc49d497b4c75c5effdd491923264523e5c0d25c209df5ba0c50d84627fad718a14f890c953bf2a180956f69549996e6917bd79ddf1a60073559cee1f8ae57e542032996c77838b536d8e62212ed210412fa4622db54daa73241773c23ce563592cd8f9161208275166fd90c5a983e6319c7dd1c48cf94411bc6611dd9f90495e5a6b0002e1ab13dee0955f305f184d0978c0a16bd9ca29a9f54099352994427e14bf61b412666fd9d5eb570368edd663dd7253bac1377b3a5610732b3c12d25ea3928d52a1e946ea51c44556de634b130391395e383da7e065c3f737a486008bf7ddb9208ac62f8c3b770de7cf24a86f4688a365ef5ee94f360e15ae4450c42aae00d94f32669432f2c89bba98");
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x027243a29d73074b7d11c0f642313cf2be017269f47e95a4623ffe761450476c"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("[INSERT].org", "dnsseed.testnet.[INSERT].org"));
        //vSeeds.push_back(CDNSSeedData("[INSERT].co", "dnsseed.testnet1.[INSERT].co"));

        // guarantees the first 2 characters, when base58 encoded, are "n1"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x19,0x57};
        // guarantees the first 2 characters, when base58 encoded, are "nx"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x19,0xE0};
        // the first character, when base58 encoded, is "9" or "c" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        // guarantees the first 2 characters, when base58 encoded, are "zz"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xC0};
        // guarantees the first 2 characters, when base58 encoded, are "ST"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, consensus.hashGenesisBlock),
            genesis.nTime,
            0,
            0
        };

        nForkStartHeight = 10;
        nForkHeightRange = 300;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        strCurrencyUnits = "REG";
        consensus.fCoinbaseMustBeProtected = false;
        consensus.nSubsidySlowStartInterval = 0;
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        // consensus.powLimit = uint256S("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f");
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.prePowLimit = consensus.powLimit;
        //   std::cout << "maxUint-UintToArith256(consensus.powLimit): " << (maxUint/UintToArith256(consensus.powLimit)).ToString() <<std::endl;
        // assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 0; // Turn off adjustment down
        consensus.nPowMaxAdjustUp = 0; // Turn off adjustment up

        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;

        consensus.nPowDifficultyBombHeight = 600000;

        pchMessageStart[0] = 0xaa;
        pchMessageStart[1] = 0xe8;
        pchMessageStart[2] = 0x3f;
        pchMessageStart[3] = 0x5f;
        nMaxTipAge = 24 * 60 * 60;

        
        const size_t N = 48, K = 5;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;
        genesis.nTime = 1482971059;
        genesis.nBits = 0x200f0f0f;
        genesis.nNonce = uint256S("0x0000000000000000000000000000000000000000000000000000000000000009");
        genesis.nSolution = ParseHex("05ffd6ad016271ade20cfce093959c3addb2079629f9f123c52ef920caa316531af5af3f");
        consensus.hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 3130;
        assert(consensus.hashGenesisBlock == uint256S("0x0575f78ee8dc057deee78ef691876e3be29833aaee5e189bb0459c087451305a"));
        nPruneAfterHeight = 1000;

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x0575f78ee8dc057deee78ef691876e3be29833aaee5e189bb0459c087451305a")),
            0,
            0,
            0
        };

        nForkStartHeight = 0;
        nForkHeightRange = 0;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);

    // Some python qa rpc tests need to enforce the coinbase consensus rule
    if (network == CBaseChainParams::REGTEST && mapArgs.count("-regtestprotectcoinbase")) {
        regTestParams.SetRegTestCoinbaseMustBeProtected();
    }
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}

