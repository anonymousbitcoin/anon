// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"
#include "base58.h"
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
        consensus.nSubsidyHalvingInterval = 134000; //1st halving occurs after block 150,740 (airdropped blocks offset)

        // Budget related
        consensus.nBudgetPaymentsStartBlock = 39420; // (coinburn block [37,000] + [2,420] (~16 days)
        consensus.nBudgetPaymentsCycleBlocks = 4380; // (blocks per day times ~ days in a month) 144 * (365/12)
        consensus.nSuperblockStartBlock = 43800; // The block at which 1st superblock goes live
        consensus.nSuperblockCycle = 4380; // (blocks per day times ~ days in a month) 144 * (365/12)
        
        // masternode
        consensus.nMasternodeMinimumConfirmations = 3;

        // governance
        consensus.nGovernanceMinQuorum = 25;
        consensus.nGovernanceFilterElements = 20000;

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
        
        // sprout burn
        consensus.zResetHeight = 48500;

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
        vSeeds.push_back(CDNSSeedData("anon1_mainnet", "45.76.29.107"));
        vSeeds.push_back(CDNSSeedData("anon2_mainnet", "72.14.185.1"));
        vSeeds.push_back(CDNSSeedData("anon3_mainnet", "198.58.124.152"));
        vSeeds.push_back(CDNSSeedData("anon4_mainnet", "50.116.27.226"));
        vSeeds.push_back(CDNSSeedData("anon5_mainnet", "198.58.103.84"));
        vSeeds.push_back(CDNSSeedData("anon6_mainnet", "45.79.13.202"));
        vSeeds.push_back(CDNSSeedData("anon7_mainnet", "91.121.69.100"));
        vSeeds.push_back(CDNSSeedData("anon8_mainnet", "188.166.31.17"));
        vSeeds.push_back(CDNSSeedData("anon9_mainnet", "128.199.54.243"));
        vSeeds.push_back(CDNSSeedData("anon10_mainnet", "128.199.55.192"));
        vSeeds.push_back(CDNSSeedData("anon11_mainnet", "68.183.196.10"));

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
        // guarantees the first 4 characters, when base58 encoded, are "ZiVK"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAB,0xD3};
        // guarantees the first 2 characters, when base58 encoded, are "SK"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAB,0x36};


        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour

        // Spork
        strSporkPubKey = "04859b86231cc91816c4ca2be433030fc0d544fcd712c3ffd9b099c496a693ddc720533acd5d9d77b6b1bf50c52cc44351bc166afb040c3725f4dcd2b5e57c83a7";

        checkpointData = {
            {
                {      0, consensus.hashGenesisBlock },
                { 49700, uint256S("0x0000005d5dc0b75986d737a5840ef41d8647f6a0234bebcba6fcc8f93de19222") },
            },
            1556450564,     // * UNIX timestamp of last checkpoint block
            51145525,          // * total number of transactions between genesis and last checkpoint
                            //   (the tx=... number in the SetBestChain debug.log lines)
            148188            // * estimated number of transactions per day after checkpoint
                            //   total number of tx / (checkpoint block height / (24 * 24))
        };


        // Setup airdrop blocks range
        nForkStartHeight = 3;
        nForkHeightRange = 16737;
        nZtransparentStartBlock = 9893 + nForkStartHeight;
        nZshieldedStartBlock = 10132 + nForkStartHeight;

        // Masternode related
        masternodeCollateralChangeBlock = 37000;
        masternodeCollateralOld = 500; 
        masternodeCollateralNew = 10000;

        //Sapling
        saplingActivationBlock = 48500;

        // Equihash algo
        eh_epoch_1 = eh200_9;
        eh_epoch_2 = eh144_5;
        eh_epoch_1_endblock = nForkStartHeight + nForkHeightRange;
        eh_epoch_2_startblock = nForkStartHeight + nForkHeightRange + 1;

        // Don't expect founders reward prior this block
        nFoundersRewardBlockStart = 37000; // actual block may vary, due to using SPORK to activate founders reward

        // choose new founders address every ~6 months.
        foundersRewardAddressPeriod = 25000;

        // Founders reward
        // ~ 6 months per each address 
        vFoundersRewardAddress = {
            "AnNhQ4bYHtdPiWfjXEr6GN4kfYxwUP8sf2g", /* main-index: 0*/
            "AnUMeF4PzFSxdhrG1dFxg9L1hgEs1xC32mA", /* main-index: 1*/
            "AnTBoW13Mf5NNPgsnDsvcbkyWt3ERNCSFUq", /* main-index: 2*/
            "AnggA4wFWzd8GMcPHiigdiU59cQKb9asq2n", /* main-index: 3*/
            "AnaEq58AGA5BvjyQQ1tiYRtTp3AJBbZn8kY", /* main-index: 4*/
            "AnhUL1HEgcvGADi2DuDEUvMBKvNcQjto3sR", /* main-index: 5*/
            "AncyG25fWsH2zzX5WhAidkAQtjfAVaZaMcj", /* main-index: 6*/
            "AnaTx6oHTggyWifNYirpeWjwUhPaLjWpXYq", /* main-index: 7*/
            "AnPcwA8LyaM79bECmDzf6hESCzuM7Bcog7d", /* main-index: 8*/
            "AnQFWT4b7foe75eUFnS3YFwcZmzMyRYTnfq", /* main-index: 9*/
            "AnRQdA7CEYXJ5qBUGRAACKA5SKBW2rDLUEc", /* main-index: 10*/
            "AnRqy2LczE5DV8RRydXLGSTRaZpszMwdGbf", /* main-index: 11*/
            "AnaY5PzxKSQH9MdevwfFxVBdX2qKiiR89YK", /* main-index: 12*/
            "Ani2bk41GKoteRfoYedoP4aaS2r888CDbic", /* main-index: 13*/
            "AnQ3G8U31m3jmqRfQjZ4XgbQEMNX4TW8Jz3", /* main-index: 14*/
            "AnZYnEcr1mat4EWDXUXu2ZY54ARJ7NmjB1A", /* main-index: 15*/
            "AnVwcAzC9ounFAzKaCDtQjhZTZCugSqvFzo", /* main-index: 16*/
            "AnVBJ5oRrdvdTWDtb41eL6cCaSU3qAfM4VY", /* main-index: 17*/
            "Anc4J1Snvs9buX8FeFMM2tZC9G1cie3wUn2", /* main-index: 18*/
            "Ani1avTH5BFDEiZRWUpaaLEeqpHVCo9dn2V"  /* main-index: 19*/
        };
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

        // Budget related
        consensus.nBudgetPaymentsStartBlock = 20;
        consensus.nBudgetPaymentsCycleBlocks = 10;
        consensus.nSuperblockStartBlock = 30; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPaymentsStartBlock
        consensus.nSuperblockCycle = 10; // Superblocks can be issued hourly on testnet

        //masternode
        consensus.nMasternodeMinimumConfirmations = 1;

        // governance
        consensus.nGovernanceMinQuorum = 3;
        consensus.nGovernanceFilterElements = 20000;

        // sprout burn
        consensus.zResetHeight = 3000;

        consensus.prePowLimit = consensus.powLimit;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.fPowAllowMinDifficultyBlocks = false;

        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        consensus.nForkStartHeight = 2;
        consensus.nForkHeightRange = 1;

        pchMessageStart[0] = 0xf7;
        pchMessageStart[1] = 0xd4;
        pchMessageStart[2] = 0xc1;
        pchMessageStart[3] = 0xb9;

        //setup airdrop blocks range
        nForkStartHeight = 2;
        nForkHeightRange = 1;
        nZtransparentStartBlock = 5;
        nZshieldedStartBlock = 6;

        // Masternode related
        masternodeCollateralChangeBlock = 1;
        masternodeCollateralOld = 500; 
        masternodeCollateralNew = 10000;

        //Sapling
        saplingActivationBlock = 100; 
        
        eh_epoch_1 = eh200_9;
        eh_epoch_2 = eh144_5;
        eh_epoch_1_endblock = nForkStartHeight + nForkHeightRange; //actual block 3
        eh_epoch_2_startblock = nForkStartHeight + nForkHeightRange + 1; //actual block 4

        

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
        // guarantees the first 4 characters, when base58 encoded, are "ZiVt"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAC,0x0C};
        // guarantees the first 2 characters, when base58 encoded, are "ST"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        // Spork
        strSporkPubKey = "04b8c57d8921b9ecd769052739f8d137104b68f68714f4d4ca3219c5664a74048bd7cbe8badbe0baeee17ec463a13f3d0f5964f56d68317fd6ef14e86380383c74";

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            ( 0, consensus.hashGenesisBlock),
            genesis.nTime,
            0,
            0
        };

        //masternode collateral
        masternodeCollateralChangeBlock = 1;
        masternodeCollateralOld = 500; 
        masternodeCollateralNew = 10000;

        // Don't expect founders reward prior this block
        nFoundersRewardBlockStart = 50; // actual block may vary, due to using SPORK to activate founders reward

        // choose new founders address every ~6 months.
        foundersRewardAddressPeriod = 25000;

        // Founders reward
        // ~ 6 months per each address 
        vFoundersRewardAddress = {
            "tAHLDZxnjpuQah28VMDnWsrK747j4kg8YzV", /* test-index: 0*/
            "tAHUxqMjG6tUSyjfpH2dTUHBk2gAoKQckdF", /* test-index: 1*/
            "tAKFCqQa5n9oCcTBVdxbj4z651CqvYDjfyS", /* test-index: 2*/
            "tAGnnENyonMAFV5c6EjnZUf9mvacWF4Qf6z", /* test-index: 3*/
            "tASoVFeMmWXrn19PkDaGK67d671jveudMMq", /* test-index: 4*/
            "tAA95SwRY47M6neVn81hoUzvANx1q4udt9q", /* test-index: 5*/
            "tAGt9vmkkyPfu8Tc6K3Fnw1cYebaUWRaDBv", /* test-index: 6*/
            "tAF1hUrjqfjoN5GGmDvNP3yG1zzigZ5u9YP", /* test-index: 7*/
            "tATSk8P4c4SVUcwA9oobkqgNoHtu8xT4mvc", /* test-index: 8*/
            "tAPVBCyvugQE91FDYRJtP5h2WvPJtVe2L3Z", /* test-index: 9*/
            "tANDfgz3pw6et13sEcW2DPcNU6rTho6RgZe", /* test-index: 10*/
            "tAPmN5w385eBWPc1LDVhHTUe2JANvSnFBuT", /* test-index: 11*/
            "tAXMHLHroZVhTv3GM9hnbtrmHCGuG6Wmm6i", /* test-index: 12*/
            "tALG9FCG8EUQ38LXku99rbtqbmgQhyvLzoH", /* test-index: 13*/
            "tAHVQvDrbayCQKc1pfakopQak87QgvhVCay", /* test-index: 14*/
            "tA9xYZ2KCBayYwPRhjYwmuytk4q5djpcD1A", /* test-index: 15*/
            "tAXDnj3JTNassQY7ffGeC3wZ3DPGybyXYqB", /* test-index: 16*/
            "tABJBqFqvjRFoPeaWP4bspxnKZmK5gnNA8G", /* test-index: 17*/
            "tAKpajpeJzp4aZUpe35ymKtAwk7WheA3aTu", /* test-index: 18*/
            "tANq6WULf5eaTSvTt7jP2XjQ85VwEoZCHJn"  /* test-index: 19*/
        };

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
        consensus.nPowAveragingWindow = 17;
        consensus.prePowLimit = consensus.powLimit;

        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 0; // Turn off adjustment down
        consensus.nPowMaxAdjustUp = 0; // Turn off adjustment up
        consensus.nPowTargetSpacing = 10 * 60;

        //masternode
        consensus.nMasternodeMinimumConfirmations = 1;

        // Budget related
        consensus.nBudgetPaymentsStartBlock = 5000;
        consensus.nBudgetPaymentsCycleBlocks = 1000;
        consensus.nSuperblockStartBlock = 6000; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPaymentsStartBlock
        consensus.nSuperblockCycle = 1000;

        // governance
        consensus.nGovernanceMinQuorum = 3;
        consensus.nGovernanceFilterElements = 20000;

         // sprout burn
        consensus.zResetHeight = -2;

        // Setup airdrop blocks range
        nForkStartHeight = 0;
        nForkHeightRange = 0;
        nZtransparentStartBlock = 0;
        nZshieldedStartBlock = 0;

        // Equihash algo
        eh_epoch_1 = eh48_5;
        eh_epoch_2 = eh48_5;
        eh_epoch_1_endblock = nForkStartHeight + nForkHeightRange;
        eh_epoch_2_startblock = nForkStartHeight + nForkHeightRange + 1;

        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;

        pchMessageStart[0] = 0x2f;
        pchMessageStart[1] = 0x54;
        pchMessageStart[2] = 0xcc;
        pchMessageStart[3] = 0x9d;
        nMaxTipAge = 24 * 60 * 60;

        //Sapling
        saplingActivationBlock = 200;

        const size_t N = 48, K = 5;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;
        genesis.nTime = 1535569200;
        genesis.nBits = 0x200f0f0f;
        genesis.nNonce = uint256S("0x0000000000000000000000000000000000000000000000000000000000000014");
        genesis.nSolution = ParseHex("082db3be12af6517f20de6265d02f5c971010ee2e2d784c525b16c21185cce784c5ec38f");
        consensus.hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18344;
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

std::string CChainParams::GetFoundersRewardAddressAtHeight(int nHeight) const {

    // asserts if node runs out of the founder addresses
    assert(nHeight >= nFoundersRewardBlockStart && ((nHeight - nFoundersRewardBlockStart) / foundersRewardAddressPeriod) < vFoundersRewardAddress.size());

    int vFoundersRewardAddressIndex = (nHeight - nFoundersRewardBlockStart) / foundersRewardAddressPeriod;

    return vFoundersRewardAddress[vFoundersRewardAddressIndex];
}

CScript CChainParams::GetFoundersRewardScriptAtHeight(int nHeight) const {
    
    CKeyID keyID;
    CBitcoinAddress addr;
    
    addr.SetString(GetFoundersRewardAddressAtHeight(nHeight).c_str());
    addr.GetKeyID(keyID);

    CScript scriptPubKey = CScript() << OP_DUP << OP_HASH160 << ToByteVector(keyID) << OP_EQUALVERIFY << OP_CHECKSIG;
    return scriptPubKey;
}

int CChainParams::GetMasternodeCollateral(int nHeight) const {

        if(nHeight >= masternodeCollateralChangeBlock)
            return masternodeCollateralNew;
        return masternodeCollateralOld;
}

bool CChainParams::isGrothActive(int nHeight) const {
        assert(saplingActivationBlock > 0);
        return nHeight >= saplingActivationBlock;
}