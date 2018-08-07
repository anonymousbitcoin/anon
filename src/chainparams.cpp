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

// Header Hash 0003e40b8e4c18bfea62fc8bc8646540ab3d83a6959b328298734466390c87c1
// Nonce 0000000000000000000000000000000000000000000000000000000000000188
//09312933fbe51fb59af3f2cb80be15f6d48e308968f3c1d235130b3eb9b81b4ed656f606c697c60a05b0ae624f9f2f82062f10a98efdd6509b0280de2967e147e16a4f2595776369d4497a2fc7d72af5492a34445aca0e64b68afebcc632dd2727f6f9c6



class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        strCurrencyUnits = "ANON";
        consensus.fCoinbaseMustBeProtected = true;

        //TODO To be decided
        consensus.nSubsidySlowStartInterval = 2;
        consensus.nSubsidyHalvingInterval = 105000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 4000;
        consensus.prePowLimit = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff9");
        // consensus.powLimit = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff9");
        consensus.powLimit = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff9");
        consensus.nPowAveragingWindow = 60; //diffuculty adjusts every 2 weeks
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
        const char* pszTimestamp = "Anonymousd8e0ce2abad2015a525f42479b7b0b324d6bb69e5b4d4310489d51c4cc22e32e";
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
        genesis.nTime    = 1532704413;
        genesis.nBits    = 0x1f07ffff;
        // genesis.nBits    = 0x1d00ffff;
        
        genesis.nNonce   = uint256S("0x000000000000000000000000000000000000000000000000000000000000068f");
        genesis.nSolution = ParseHex("18469d916cd847f65e7013d622db5a95991165544d31874eb6216cdca2a04d50c927fdc24a57c3a1c4eede522cc197ed23262078e9b69154af0ce938a35689e6f25da872a2fa0099acef372395d79c6874ae9c61d8aa1b24706c7b89507ede0c85825d17");

        consensus.hashGenesisBlock = genesis.GetHash();
        // assert(consensus.hashGenesisBlock == uint256S("0x0002aee7e57553028a6156fe28f6a74bfacc0a8c0cd45f19027b6adc0a628044"));
        assert(consensus.hashGenesisBlock == uint256S("0x0006e2f1792ad5a97c2a266c2d65bbd53b0f44b153db1f35d40acbb7d24ae93b"));
        assert(genesis.hashMerkleRoot == uint256S("0xe50058ee0a974a2e8562fe45782bc1a104af045df1b3d5c0de4aea2e972d1d59"));
        // assert(genesis.hashMerkleRoot == uint256S("19612bcf00ea7611d315d7f43554fa983c6e8c30cba17e52c679e0e80abf7d42"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // TODO: setup a DNSSeed
        //vSeeds.push_back(CDNSSeedData("[INSERT].org", "dnsseed.[INSERT].org"));
        //vSeeds.push_back(CDNSSeedData("[INSERT].co", "dnsseed.[INSERT].co"));
        
        // guarantees the first 2 characters, when base58 encoded, are "bK"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x13,0x53};

        // guarantees the first 2 characters, when base58 encoded, are "3Z"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x53,0x89};

        // the first character, when base58 encoded, is "5" or "K" or "L" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0x80};

        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x88,0xB2,0x1E};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x88,0xAD,0xE4};
;
        // guarantees the first 2 characters, when base58 encoded, are "B3"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x05,0xA8};

        // guarantees the first 2 characters, when base58 encoded, are "LY"
        base58Prefixes[ZCSPENDING_KEY]     = {0x0B, 0x01};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
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
        consensus.powLimit = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

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
        // genesis.nTime = 1531350314;
        // genesis.nBits = 0x2007ffff;
        // genesis.nNonce = uint256S("0x0000000000000000000000000000000000000000000000000000000000000003");
        // genesis.nSolution = ParseHex("0152b04ab791857d06182362176679efc7937ed3c4116a8ce3bf1d66750c50336048cc842f72591e94181a3eab9020b4dc01e16942cfe2c4e079847b75ad6e269617e2f5d2a12547d8c621d0cbce9e02605a0a3403f9527b798255794efcd2b36ae707fe62dbde55ff04c961ec1b498fe2d374a47d0646fe95cdf3ba85ef0ff740be0e143a476b36b374f4765031799c3b086914acadb84a51b9cbe7a3a42af03df83d481075f54502b34777ba430bb97253c0af8ba1f0114180706f762b7ec9f7cd9d2aa7f5f2f3cb2ac31f17072f789a3742819e40d8e4ab357dfbd50873d02c0599c9d5706f591423e97e9f391b11b72a24fc5d1f2b2f6499bd8f108f876996c7011385b7015983d7262b28f89b2a5b1f38b6e88dc9297b55b7448216787c19bead0f79ec17f5aae4e9d4454ed398c17ffbacb03ce4f6753fbe452cfd174f556b38eed7b4bcdfb0238990cbf4b377038cc88db3d3ab2eb397c77859646472459d971c331671bad3572892df89c552d5541b73c2267d3ccc520ff08d3a63ccf846955e03898e4dccef56d41ae4aa251e19a8221e0cadad4e83942cfdd74a3be1bcc6b70778758b5d93cdfd4e0a808781ffe4883b446b522c1232af94c2e4273fce08a34231ff7ab9fecafe3adf0a88173189975b7cd9fb03693342929ec28dff0cf668da3f51bba8f4b19b6426b686f9e6fe10b8f58bfa12d115edd3a6df539f7851f02c9379ec7d5759fb0519a04a82495b9cb7294d68439355b3f32d49ffa8fc175561620811bf89a6f1131ec16af141e04fb788202d722c589297bbaf751f847d51c7aa66626f15cc0715d7efc138575771c5ed07d9a37838ee13b03380f2237b5f606115c8ed6eb7837f4af0a38962edf78387182bbe0664d107097666b78c42c6a296e04539d6d44d9cd4a628726445b9a2b532594d22269fc91cb19e0249ad8dd01d12876c517246c3655fa57c0bf869140cfc754e4c84234af466435879a6934cdb6c324d5e026a2e67c9b44a73baca64d189f308e676147c7f19166da2e4db9622aac0d8a4077a73b6a736867f59df08fcb0511e0343851fc38415a8ab7e130d6ffa255d6fb944bed45fca0f86cc28db84e883bef7d3dfc3f7170484f96037dc9bf53c63a0ce7f3113499e9cb816241ad3ff16184cf3d810a2d2602ddf62102d7c05cb0abd89f5746465e1c523a76ff83c330e9a24b74d72290681918e99d03cf6f474736e6ada39d4833c604113087517dc6ad4ad6c3855ce47cc28168f1b7748fb1dcf0ee1e5c9df850c8c867b0c45bbf6468396a681116807f45311f5fcdd67327cdfde4815b3659853805b3836d0b95cfc5d407d08729ec66e727c109fda5c1824242eb0a5a4ab8c2fd4bd39b813f3765c9cdab526294c228ebb9323ebba472da0c3135abe3937d70f02a7b4b414cf64b2a0cd70b4e5d8879503dd7692501af91e3773ec6a25a77aa1cfa36ff701782c97a4140546003ff10f787910bfb19d9b17a9c20258fd11c9203c517b460f708efc0c33f659d5c02e111ebb16990a5e73e5ec635e8d8e8574a361574b8e331bdb8a7a0c849152e30f09535f510550c4514621f563d4c00f14796f483e1360a3f170e1bab9d495e973ddcf46d03169a6b57efa3897e8bf24426de755f97792bf7003045a2e0cb3ecf613cfc191df0344421b75cbbc407e0a75e1dfe9bb8b4de147ac9167794437976b7da26608898148120c5363439111d2fb2df8e8d4e50d196b1642d5ffdbe83c5f796891d7a7f8d9e16f9479b175076a535c39640fa1a07fb5b0377629862fdd92fba7090fb905020a8a20804012da9d6a998918a60e4e9b0859f96966119decab0000f30ef22a4216b6bc58f01e43dc2f3a0e211de07d6302f5724bb24c0a5c2050");
        consensus.hashGenesisBlock = genesis.GetHash();
        // assert(consensus.hashGenesisBlock == uint256S("0x064c5de072fb7106f34f1a0d2f1ef11aecdb2f140ab6dbbf24d93a5124402e6f"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //Final seed locations to be decided
        //vSeeds.push_back(CDNSSeedData("[INSERT].org", "dnsseed.testnet.[INSERT].org"));
        //vSeeds.push_back(CDNSSeedData("[INSERT].co", "dnsseed.testnet1.[INSERT].co"));

        // guarantees the first 2 characters, when base58 encoded, are "6W"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x03,0x1A};

        // guarantees the first 2 characters, when base58 encoded, are "jQ"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x17,0xE0};

        // the first character, when base58 encoded, is "9" or "c" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};

         // guarantees the first 2 characters, when base58 encoded, are "vX" or "vW"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x14,0xff};

        // guarantees the first 2 characters, when base58 encoded, are "2B"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAA};

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
        const char* pszTimestamp = "Zclassic860413afe207aa173afee4fcfa9166dc745651c754a41ea8f155646f5aa828ac";

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
        // assert(consensus.hashGenesisBlock == uint256S("0x0575f78ee8dc057deee78ef691876e3be29833aaee5e189bb0459c087451305a"));
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

