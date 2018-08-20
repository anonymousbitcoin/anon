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
        consensus.nSubsidySlowStartInterval = 1;
        consensus.nSubsidyHalvingInterval = 134000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 4000;
        consensus.prePowLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimit = uint256S("07ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 17; //diffuculty adjusts every 2 weeks
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
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
        // const size_t N = 200, K = 9;
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
        genesis.nTime    = 1532704413;
        genesis.nBits    = 0x1f07ffff;
        // genesis.nBits    = 0x1d00ffff;

        genesis.nNonce   = uint256S("00000000000000000000000000000000000000000000000000000000000011cc");
        genesis.nSolution = ParseHex("003db0e442d767a8d220107853e9eb2ba9aafe7c3d04924a798214caf5f3fee0ebc6f8e5726b0038b6bb1519fd91e35fd2c5af0794fc48f5c535a0bcddecbc1f55269c0acca57d35d8e449913b399e5c393cb2e800a3957f580ec82be61b208d104e2080896057f1532bc84caa9369cbf7decb7424bb4fd8fd8eae30414c01dd58de7bc863c2f3146165d99c04cca734b0549f679a8f1b8cda3e76d41137331e51624a0798bfdfb505a8c53b3f4f3868ffb5284d5a73f2574d4a1c7c421c1db23fa751ef5cc810c7bc24eb520e13e13c4b3711f7379ca96b28ffc091d8d05d5670daab87d7e8ed1feb4a4ed8ce40579c2b63935af87c837452dcaa290b4c76fdd004be50cba502ab4246226985377426ef24ffc666455307f99f5d13307bf02fa758ecdfdbb915b015de749c2e43541eb1f8c765b92db9543029822cb8130b430bbb1aa7dcf4d427d7a51240a77788780115feefd408668beed1a86c78deff12a9099b372c3e5a3fc0f4a5c9339d5765e5cb52906a9c2775c53b07827b31cf0a16bb6e14b0cdbdaa211b40a39e7b7529a84347ad4b688896d0844c4e688d3528f6d4964b11a814ab5fd957b1ea4bf4c7772a0cd63660d1c5ee124c80b949a833277628a1848761c2986f46742de413892edb96b5e3cff5fc56c249ea67071d8d5b5e471f2989841223ed3bf1124251d6e1f356300cda5be302d4483ec49cc6bf4646e33682460d40f63f7cd58507757d9e431b8cfd4ec1733edc61fa1d8cdd3f6dd70d5704ee51c9328738ee515cc8fdb6c2db4119e8a929c0bbbdbae30f53c47f74b1e6fd24ea3ad932b06a04a792694416630f281891b9709b91cadb03fa7ba74106a66c8652a12af463c496e2372152247a73d8301f45a3beb7258791940ec44da4264d8dec0bba91113990feb38a375b0df68ab5a87454dcddf717b22847004ca264ab28c2bf583e425f53a22e5969062f9cb71fc3fe553f481067f2c29253af76e56dc1e570c9312b41b27cd4ea86abe388749c24b68399409b96c8173a70c5f3bb2c1d6f96a5c8433c74bcb2c5709ed4d401d27995270780eaf8eb51b240e63e5ae679d9edb002dc8d358459b3fb09f0f19b81ea20faac9a3a0b110cdb9a06eed62e6cf692377711423f2e45f272a1820eb998ed136a16c79d747121e7ee876d53ff9468d80c54a336fde0e51f59c3c3dbb62408120eaafa18ef3c00b53ce710c993fedb04901667e0fa9d2e59713d12cadb9d968b80d3b62c84114ca52e453b1bbd612014b071f6eea8e191711a54bd377793b543c53b197c0cd3237ae14b1e2b56fd4115dc7723421bd394d787267ad189cb2b5d79c517a27121ea4ee0aa26c6c780199e6ecbaa8fd64d5cb9c5dd58eeb7059c4d6d2b982082b7764d9c14eba566d26eefd82be537de2b5afa00783614bed55f9db5e3a4d74f56595707765a5f3a059ccf2c1796777567e473c3dfe18fd2d0bdd7839d023eb20f3bc4660ecf0f154acc78feb1bf1c1cb0f70fcbcb7d6f17d57b5dd3d58a4ad573e2465b1c0f360113904daa9759e7ccdb70b981e5ae0599b5d18ceb178f3ea1e14e846fad2d3213eb61991538c4d7bd6a03fa86e387e079cdae66026ba92989bd5b63f991f31ee7c5bc8b572b175cbb576e17d951ab5a8ddbaeb501ca3a527014b42bc3c0e81575797cb3d07e9f90312dce9d2ffdeb9fc7b45ed62612d4845e0111955d771b613fe7501b5ff722087602c6724da2666096b97c6a0cd3cd28b15fb5f9f357e66ad621a60680940223082e1b73038c8a456f6964f327cb3c1a6131babb4e0e6a53bf284b4a0facd6d5a535dc1202a547b5a98420932dbff7694c2d7080e6386d6f77e2a7b495951927782962539a8dc135cbe538cb4ab08da59c3cfa9a");

        consensus.hashGenesisBlock = genesis.GetHash();
   
        assert(consensus.hashGenesisBlock == uint256S("0x0004284da5ba543dd9cc5c3589304a8fc74e8eb59160581c10fc612636cc15f2"));
        assert(genesis.hashMerkleRoot == uint256S("0x19612bcf00ea7611d315d7f43554fa983c6e8c30cba17e52c679e0e80abf7d42"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // TODO: setup a DNSSeed

        // guarantees the first 2 characters, when base58 encoded, are "An"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x05,0x82};

        // guarantees the first 2 characters, when base58 encoded, are "3Z"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x53,0x89};

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

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        checkpointData = {
            {
                {      0, consensus.hashGenesisBlock }
                // {  30000, uint256S("0x000000005c2ad200c3c7c8e627f67b306659efca1268c9bb014335fdadc0c392") },
                // { 160000, uint256S("0x000000065093005a1a46ee95d6d66c2b07008220ca64dd3b3a93bbd1945480c0") },
                // { 272992, uint256S("0x0102a4e1353149c9221ebd7c45b3170ea4b8a7d1cf4075b3bc50c440fb9ebdd6") },
                // { 273992, uint256S("0x000822e42b2128a296b704c273a4c3f0e707b06305d2be521cf7577ef67e476a") },
                // { 274992, uint256S("0x03e91186991b31044179b82e88ee06c2205e3304a525d130070153fde4aaf414") },
                // { 275992, uint256S("0x02bc3f08cec58ac79b35a29161b43cd301696e6e6421d704314d08a35b2169db") },
                // { 276992, uint256S("0x0046665bbb64b609935ed0fd184d6c42657dc5c36b16ed1e59ecc2fcb0508d32") },
                // { 277992, uint256S("0x0111d3ffc37fb3474ed2a6e3e9bfc53ee0d7cc0413a732f19d59061700f6b842") },
                // { 278458, uint256S("0x0747cecedfd30754323c5afdaef547d5e90fd50485370ebf12dd33bbac61faaa") },
                // { 279500, uint256S("0x0000000082ece76130c82337903b14a2109fab2dcd2d153fcdbf48a0054a694b") },
            },
            0,     // * UNIX timestamp of last checkpoint block
            0,       // * total number of transactions between genesis and last checkpoint
                            //   (the tx=... number in the SetBestChain debug.log lines)
            50            // * estimated number of transactions per day after checkpoint
                            //   total number of tx / (checkpoint block height / (24 * 24))
        };

        nForkStartHeight = 0;
        nForkHeightRange = 0;
        zUtxoMiningStartBlock = 0;

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
        consensus.nMinerConfirmationWindow = 17; // nPowTargetTimespan / nPowTargetSpacing

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
        genesis.nTime = 1534431068;
        genesis.nBits = 0x2007ffff;
        genesis.nNonce = uint256S("0x0000000000000000000000000000000000000000000000000000000000000008");
        genesis.nSolution = ParseHex("0018f32d1b2232ff3adf1122bcc5702594042f8d23391595140ced5a817ea7c6fc3741b7af1ac31e21d32138ec934ca9635557305610cfb739ad99e38fc11b428faf67bc2e712ddabe356513fd58bea1507ef48306e5d192a7d691f13cc1e190016c4a8ceaf60c39c02cfdc37fee8f6dbcbdd8f32031541f5af783590e9f12988e061620b2f977ae843d13d665e12a26966a9c23d1baf1425ebd29b124b2d44f174e14dbabd9184405f15452920c9c278cf7315e57e02bd10d9df2833c0769403dd6b09ee9deff3a0949f62466ca30b67bf50e3115d2da5e205145ab242a12594eeb0488bfc62e126cab537c0b91193b9fe1cc4f4b9b69292d1d717d25677d42472a65b95ddef7e59170c39a01c598988938e7cabe543959afd3d27500986927718fe2db92c82d4c1388421a64cf4601e4f998bcfa621e8c9d4dca52479c9ef09fc65348f7e62eed33b605af59f5f50c00a2e0aca66ea725af60d47227c26f36cdbfda2390250c319d926437eb717fa626734e7d2356fa1ae4820e0e29bd49fc28b5edbe343479f06bf569f87481461c8a05530807dd9862ea64176a3ac7ee6dfc7aa77d01b18f1a2ccd4e806ca0b0d99364684880ae15495607005066e8b034a1c5d31463bcac1d322fbbfb54f61d6dbc6ff00af58ccb78347096b448e97be11cea4f5a39ee7d48707cd7aee07796cdc2791f400b1f161500d03476ff3e79a9f9f0417efc24954966b53d5d600dbfd528eb4bd1286cd529fcbb5d5bdbdb37dfed6603de990b1ad33363eb67386fe37a8aef34d81c7f87582efe9456f710a7e04516e59b37b532e8a8f7ef520d7666e1ea609fad2cffe6dc58c146726d291e260312290ea6b8b1faf1f871e86626deff2292df3a0a7d12ca1719f74af1faebe063bfbcd15cb1d871f1d42433d15c8aef1bd515905e67155dd0f78e2cffb2bd3f00840c2b9f2cd485bbfab1362b3b033391331ce2fc2fae0194e1df47d7c6ca84f63b2d2c5e206858114f087a3add8edf9b3b25a47263ebc4d9e89da1948bb8272cbed8c4cd2939b257b52ad85bbcb2c3423a8498016029220652ce03ccf8f62a4565c5a37244bcb1cd2743a7181f1acd0f1203e390b3ae2ec95709930ef60c15bc75604ddc18c88692b4352c2e8e5aed7685312c59c6525d70d661d63ab2dbb1c8e6f1111ef56b6606c98f7dea308ffbf66db311e4633f59a59b3d1961263d45a326658581bd20460fe03fa62599d4fe3779198f924fa6e0a69bf39022642e7dd560ed487fd14a1d26f3965e5ef99cff22a416b4a812cdbc1979a7d90b437b9030368b5bb6b9422912adb7f1d6f03685f01bb18a423c387133fec9c26114174ce946dc3b2541261492f95fced30153d7e59d81f50cb5953e6e2b804e9ef71bc9947fdde2fc38d4e154fda2b01e3ab97603bb6257e085f98cd28cb08ac06fae4e5391564ebf071c61f9c1a21d892c84e10e0947a8a0510bf8a9ee0719ad7a46df973b5bc570d73d3d242d01649f00ef1c6015daa4216abf5fc1d4d844cf3cad4841b3a1e60a89438baf523aaedc711497ddb5d5fba55a1e692b21f863d4ae0e55c2d33e83db54c6822509283344460bd509228f18af2710e1a23652a969098f34700dc00df44274495b0312db0a524f4034d355d6fa96687a0cf3ccead268e325c60bd1fcea65739dbf013229fc2e0bedecf1913db8d5b5b2f371701c6932a915980320e932e9219e2ec39719763a74d0e73daa569bbf534c242d0b2c9a593da21aaae57beb2e170eb05e950217e06f84020ed1768bea55028bc6473d4d15b64f01269affefbacf2bc8ecfba3fc5ed64a4262385e33322707ca9abb10bf62f79c9417c1e06bb9ce8b34468b2961d2fbb95e1521f2aa442574f3b5d58073901559");
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x01064a94d893deab5198592c9a950be8fdbb9ca7e9d512803a4872e176e116fb"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("anon1", "50.116.31.254"));
        vSeeds.push_back(CDNSSeedData("anon2", "45.56.70.130"));
        vSeeds.push_back(CDNSSeedData("anon3", "69.164.196.203"));
        vSeeds.push_back(CDNSSeedData("anon4", "23.239.30.210"));
        vSeeds.push_back(CDNSSeedData("anon5", "45.79.6.196"));
        vSeeds.push_back(CDNSSeedData("anon6", "66.228.52.134"));

        // guarantees the first 2 characters, when base58 encoded, are "tA"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1C,0xCE};
        // guarantees the first 2 characters, when base58 encoded, are "t2"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBA};
        // the first character, when base58 encoded, is "9" or "c" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0x80};
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

        nForkStartHeight = 0;
        nForkHeightRange = 0;
        zUtxoMiningStartBlock = 0;
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
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

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




