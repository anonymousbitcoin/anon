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
        consensus.nSubsidySlowStartInterval = 1;
        consensus.nSubsidyHalvingInterval = 134000; 
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 4000;
        consensus.prePowLimit = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimit = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 17; //difficulty adjusts every block
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

        consensus.nForkStartHeight = 3;
        consensus.nForkHeightRange = 16737;

        pchMessageStart[0] = 0x83;
        pchMessageStart[1] = 0xD8;
        pchMessageStart[2] = 0x47;
        pchMessageStart[3] = 0xA7;
        vAlertPubKey = ParseHex("04f2cd746e629ffd320a81287474c98c2ad15d15b0a210b0144edcd8f3e1301c6311fd751fa34ba17d88090374cfec7cd9aaca55a5a0c4456511acc01b922005de");
        nDefaultPort = 33130;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 100000;


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
        genesis.nTime    = 1536537600;
        genesis.nBits    = 0x2007ffff;

        genesis.nNonce   = uint256S("0x0000000000000000000000000000000000000000000000000000000000000046");
        genesis.nSolution = ParseHex("01d27e3621af0d698439e61ad0eef7d61ebe392ab7ab430b484337b3ace0fe1ce0781f1485a6f1301d7f047fc8e2f1f50ef9041b4b6b5631a202c23986ff94e0cd56ca258d8e9a6b70b70e21dc5319733dca7346b37cd8f594b6db26db11754b89a31936");

        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x053a237d7ad7106e341a403286604df55bfe6f301fc9fff03a06f81c8c565b34"));
        assert(genesis.hashMerkleRoot == uint256S("0xe50058ee0a974a2e8562fe45782bc1a104af045df1b3d5c0de4aea2e972d1d59"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("anon0-mainnet", "149.28.224.182"));
        vSeeds.push_back(CDNSSeedData("anon1-mainnet", "149.28.224.254"));
        vSeeds.push_back(CDNSSeedData("anon2-mainnet", "108.61.159.183"));
        vSeeds.push_back(CDNSSeedData("anon3-mainnet", "139.162.88.43"));
        vSeeds.push_back(CDNSSeedData("anon5-mainnet", "172.104.185.21"));
        vSeeds.push_back(CDNSSeedData("anon7-mainnet", "139.162.180.228"));
        vSeeds.push_back(CDNSSeedData("anon9-mainnet", "72.14.184.145"));
        vSeeds.push_back(CDNSSeedData("anon11-mainnet", "149.202.74.156 "));
        vSeeds.push_back(CDNSSeedData("anon12-mainnet", "91.121.69.100"));
        vSeeds.push_back(CDNSSeedData("anon13-mainnet", "45.76.241.37"));
        vSeeds.push_back(CDNSSeedData("anon14-mainnet", "99.42.79.176"));

        vSeeds.push_back(CDNSSeedData("anon14-mainnet", "149.28.224.182"));
        vSeeds.push_back(CDNSSeedData("anon14-mainnet", "140.82.45.19"));
        vSeeds.push_back(CDNSSeedData("anon14-mainnet", "45.76.1.51"));
        vSeeds.push_back(CDNSSeedData("anon14-mainnet", "149.28.224.254"));
        vSeeds.push_back(CDNSSeedData("anon14-mainnet", "45.63.9.86"));
        vSeeds.push_back(CDNSSeedData("anon14-mainnet", "108.61.159.183"));

        vSeeds.push_back(CDNSSeedData("anon15-mainnet", "149.28.40.191"));
        vSeeds.push_back(CDNSSeedData("anon16-mainnet", "207.148.17.101"));
        vSeeds.push_back(CDNSSeedData("anon17-mainnet", "45.77.103.39"));
        vSeeds.push_back(CDNSSeedData("anon18-mainnet", "149.28.60.58"));
        vSeeds.push_back(CDNSSeedData("anon19-mainnet", "45.76.8.166"));
        vSeeds.push_back(CDNSSeedData("anon20-mainnet", "207.246.95.8"));
                

        // guarantees the first 2 characters, when base58 encoded, are "An"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x05,0x82};

        // guarantees the first 2 characters, when base58 encoded, are "3Z"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x53,0x89};

        // the first character, when base58 encoded, is "5" or "K" or "L" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0x80};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x88,0xB2,0x1E};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x88,0xAD,0xE4};
        // guarantees the first 2 characters, when base58 encoded, are "zc"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0x9A};
        // guarantees the first 2 characters, when base58 encoded, are "SK"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAB,0x36};


        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        checkpointData = {
            {
                {      0, consensus.hashGenesisBlock }
                // {  30000, uint256S("0x000000005c2ad200c3c7c8e627f67b306659efca1268c9bb014335fdadc0c392") },
            },
            0,     // * UNIX timestamp of last checkpoint block
            0,       // * total number of transactions between genesis and last checkpoint
                            //   (the tx=... number in the SetBestChain debug.log lines)
            50            // * estimated number of transactions per day after checkpoint
                            //   total number of tx / (checkpoint block height / (24 * 24))
        };

        nForkStartHeight = 3;
        nForkHeightRange = 16737;
        nZtransparentStartBlock = 9893 + nForkStartHeight;
        nZshieldedStartBlock = 10132 + nForkStartHeight;

        eh_epoch_1 = eh200_9;
        eh_epoch_2 = eh144_5;
        eh_epoch_1_endblock = nForkStartHeight + nForkHeightRange;
        eh_epoch_2_startblock = nForkStartHeight + nForkHeightRange + 1;
 
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
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.fPowAllowMinDifficultyBlocks = false;

        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        pchMessageStart[0] = 0xd3;
        pchMessageStart[1] = 0xf4;
        pchMessageStart[2] = 0xf3;
        pchMessageStart[3] = 0x77;
        
        eh_epoch_1 = eh200_9;
        eh_epoch_2 = eh144_5;
        eh_epoch_1_endblock = nForkStartHeight + nForkHeightRange + 999999998;
        eh_epoch_2_startblock = nForkStartHeight + nForkHeightRange + 999999999;


        vAlertPubKey = ParseHex("048679fb891b15d0cada9692047fd0ae26ad8bfb83fabddbb50334ee5bc0683294deb410be20513c5af6e7b9cec717ade82b27080ee6ef9a245c36a795ab044bb3");
        nDefaultPort = 33129;
        nPruneAfterHeight = 1000;


        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1537347600;
        genesis.nBits = 0x2007ffff;
        genesis.nNonce = uint256S("0x000000000000000000000000000000000000000000000000000000000000000f");
        genesis.nSolution = ParseHex("1a2a4e95433a1db4889ae7a5883d8e665ec2923e14876bbb902ccf3d7d5bd19da42bfc0d92a5fab90bab8542157c2d35fe6226922d41f060e6439a934781c599eef63c0dd6e6be2fded32e61296eb311211a25253f15bda68b9844794432bcd473717655");
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x07d2dd91f13803386aa0c05afd2adfcc6f911c5571a330a1118f6a0b3e0b2073"));

        vFixedSeeds.clear();
        vSeeds.clear();

        vSeeds.push_back(CDNSSeedData("testnet_node1", "198.58.97.186"));
        vSeeds.push_back(CDNSSeedData("testnet_node2", "45.33.13.94"));
        vSeeds.push_back(CDNSSeedData("testnet_node3", "45.56.69.11"));
        vSeeds.push_back(CDNSSeedData("testnet_node4", "96.126.120.121"));
        vSeeds.push_back(CDNSSeedData("testnet_node5", "69.164.195.11"));


        // guarantees the first 2 characters, when base58 encoded, are "tA"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1C,0xCE};
        // guarantees the first 2 characters, when base58 encoded, are "t2"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBA};
        // the first character, when base58 encoded, is "9" or "c" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        // guarantees the first 2 characters, when base58 encoded, are "zt"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xB6};
        // guarantees the first 2 characters, when base58 encoded, are "ST"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, consensus.hashGenesisBlock),
            genesis.nTime,
            0,
            0
        };

        nForkStartHeight = 2;
        nForkHeightRange = 1;
        nZtransparentStartBlock = 5;
        nZshieldedStartBlock = 6;
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
        genesis.nTime = 1535569200;
        genesis.nBits = 0x200f0f0f;
        genesis.nNonce = uint256S("0x0000000000000000000000000000000000000000000000000000000000000014");
        genesis.nSolution = ParseHex("082db3be12af6517f20de6265d02f5c971010ee2e2d784c525b16c21185cce784c5ec38f");
        consensus.hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 3130;
        assert(consensus.hashGenesisBlock == uint256S("0x0405af839b4b9e53bae1f951e76b0e0a33d1ca6901fc264893adb375cc04a410"));
        nPruneAfterHeight = 1000;

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = (Checkpoints::CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x0405af839b4b9e53bae1f951e76b0e0a33d1ca6901fc264893adb375cc04a410")),
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

int validEHparameterList(EHparameters *ehparams, unsigned long blockheight, const CChainParams& params){
    //if in overlap period, there will be two valid solutions, else 1.
    //The upcoming version of EH is preferred so will always be first element
    //returns number of elements in list
    if(blockheight>=params.eh_epoch_2_start() && blockheight>params.eh_epoch_1_end()){
        ehparams[0]=params.eh_epoch_2_params();
        return 1;
    }
    if(blockheight<params.eh_epoch_2_start()){
        ehparams[0]=params.eh_epoch_1_params();
        return 1;
    }
    ehparams[0]=params.eh_epoch_2_params();
    ehparams[1]=params.eh_epoch_1_params();
    return 2;
}
