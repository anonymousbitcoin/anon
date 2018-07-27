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
        consensus.nSubsidyHalvingInterval = 105000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 4000;
        consensus.prePowLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimit = uint256S("07ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 2016; //diffuculty adjusts every 2 weeks
        // assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 32; // 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // 16% adjustment up
        consensus.nPowTargetSpacing = 10 * 60; // time between blocks (sec)
        consensus.fPowAllowMinDifficultyBlocks = false;

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

        // guarantees the first 2 characters, when base58 encoded, are "3Y"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x53,0x1A};

        // guarantees the first 2 characters, when base58 encoded, are "ax"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x53,0x89};

        // the first character, when base58 encoded, is "5" or "K" or "L" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0x80};

        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x88,0xB2,0x1E};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x88,0xAD,0xE4};
;
        // guarantees the first 2 characters, when base58 encoded, are "4g"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x53,0xA8};

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

        nAirdropStartHeight = 0;
        nAirdropHeightRange = 0;
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
        consensus.powLimit = uint256S("07ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.prePowLimit = consensus.powLimit;
        // assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.fPowAllowMinDifficultyBlocks = true;

        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        pchMessageStart[0] = 0x7a;
        pchMessageStart[1] = 0x74;
        pchMessageStart[2] = 0x8d;
        pchMessageStart[3] = 0x38;

        vAlertPubKey = ParseHex("048679fb891b15d0cada9692047fd0ae26ad8bfb83fabddbb50334ee5bc0683294deb410be20513c5af6e7b9cec717ade82b27080ee6ef9a245c36a795ab044bb3");
        nDefaultPort = 33127;
        nPruneAfterHeight = 1000;


        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1531350314;
        genesis.nBits = 0x2007ffff;
        genesis.nNonce = uint256S("0x0000000000000000000000000000000000000000000000000000000000000003");
        genesis.nSolution = ParseHex("0152b04ab791857d06182362176679efc7937ed3c4116a8ce3bf1d66750c50336048cc842f72591e94181a3eab9020b4dc01e16942cfe2c4e079847b75ad6e269617e2f5d2a12547d8c621d0cbce9e02605a0a3403f9527b798255794efcd2b36ae707fe62dbde55ff04c961ec1b498fe2d374a47d0646fe95cdf3ba85ef0ff740be0e143a476b36b374f4765031799c3b086914acadb84a51b9cbe7a3a42af03df83d481075f54502b34777ba430bb97253c0af8ba1f0114180706f762b7ec9f7cd9d2aa7f5f2f3cb2ac31f17072f789a3742819e40d8e4ab357dfbd50873d02c0599c9d5706f591423e97e9f391b11b72a24fc5d1f2b2f6499bd8f108f876996c7011385b7015983d7262b28f89b2a5b1f38b6e88dc9297b55b7448216787c19bead0f79ec17f5aae4e9d4454ed398c17ffbacb03ce4f6753fbe452cfd174f556b38eed7b4bcdfb0238990cbf4b377038cc88db3d3ab2eb397c77859646472459d971c331671bad3572892df89c552d5541b73c2267d3ccc520ff08d3a63ccf846955e03898e4dccef56d41ae4aa251e19a8221e0cadad4e83942cfdd74a3be1bcc6b70778758b5d93cdfd4e0a808781ffe4883b446b522c1232af94c2e4273fce08a34231ff7ab9fecafe3adf0a88173189975b7cd9fb03693342929ec28dff0cf668da3f51bba8f4b19b6426b686f9e6fe10b8f58bfa12d115edd3a6df539f7851f02c9379ec7d5759fb0519a04a82495b9cb7294d68439355b3f32d49ffa8fc175561620811bf89a6f1131ec16af141e04fb788202d722c589297bbaf751f847d51c7aa66626f15cc0715d7efc138575771c5ed07d9a37838ee13b03380f2237b5f606115c8ed6eb7837f4af0a38962edf78387182bbe0664d107097666b78c42c6a296e04539d6d44d9cd4a628726445b9a2b532594d22269fc91cb19e0249ad8dd01d12876c517246c3655fa57c0bf869140cfc754e4c84234af466435879a6934cdb6c324d5e026a2e67c9b44a73baca64d189f308e676147c7f19166da2e4db9622aac0d8a4077a73b6a736867f59df08fcb0511e0343851fc38415a8ab7e130d6ffa255d6fb944bed45fca0f86cc28db84e883bef7d3dfc3f7170484f96037dc9bf53c63a0ce7f3113499e9cb816241ad3ff16184cf3d810a2d2602ddf62102d7c05cb0abd89f5746465e1c523a76ff83c330e9a24b74d72290681918e99d03cf6f474736e6ada39d4833c604113087517dc6ad4ad6c3855ce47cc28168f1b7748fb1dcf0ee1e5c9df850c8c867b0c45bbf6468396a681116807f45311f5fcdd67327cdfde4815b3659853805b3836d0b95cfc5d407d08729ec66e727c109fda5c1824242eb0a5a4ab8c2fd4bd39b813f3765c9cdab526294c228ebb9323ebba472da0c3135abe3937d70f02a7b4b414cf64b2a0cd70b4e5d8879503dd7692501af91e3773ec6a25a77aa1cfa36ff701782c97a4140546003ff10f787910bfb19d9b17a9c20258fd11c9203c517b460f708efc0c33f659d5c02e111ebb16990a5e73e5ec635e8d8e8574a361574b8e331bdb8a7a0c849152e30f09535f510550c4514621f563d4c00f14796f483e1360a3f170e1bab9d495e973ddcf46d03169a6b57efa3897e8bf24426de755f97792bf7003045a2e0cb3ecf613cfc191df0344421b75cbbc407e0a75e1dfe9bb8b4de147ac9167794437976b7da26608898148120c5363439111d2fb2df8e8d4e50d196b1642d5ffdbe83c5f796891d7a7f8d9e16f9479b175076a535c39640fa1a07fb5b0377629862fdd92fba7090fb905020a8a20804012da9d6a998918a60e4e9b0859f96966119decab0000f30ef22a4216b6bc58f01e43dc2f3a0e211de07d6302f5724bb24c0a5c2050");
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x064c5de072fb7106f34f1a0d2f1ef11aecdb2f140ab6dbbf24d93a5124402e6f"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //Final seed locations to be decided
        //vSeeds.push_back(CDNSSeedData("[INSERT].org", "dnsseed.testnet.[INSERT].org"));
        //vSeeds.push_back(CDNSSeedData("[INSERT].co", "dnsseed.testnet1.[INSERT].co"));

        // guarantees the first 2 characters, when base58 encoded, are "6W"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x03,0x1A};

        // guarantees the first 2 characters, when base58 encoded, are ""
        base58Prefixes[SCRIPT_ADDRESS]     = {0x17,0xE0};

        // the first character, when base58 encoded, is "9" or "c" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};

         // guarantees the first 2 characters, when base58 encoded, are "vX" or "vW"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x14,0xff};

        // guarantees the first 2 characters, when base58 encoded, are ""
        base58Prefixes[ZCSPENDING_KEY]     = {0xfa,0x07};

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

        nAirdropStartHeight = 0;
        nAirdropHeightRange = 0;
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
        consensus.nSubsidyHalvingInterval = 105000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f");
        consensus.prePowLimit = consensus.powLimit;
        // assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 0; // Turn off adjustment down
        consensus.nPowMaxAdjustUp = 0; // Turn off adjustment up

        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;

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

        nAirdropStartHeight = 0;
        nAirdropHeightRange = 0;
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

