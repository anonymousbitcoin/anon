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
        // const size_t N = 144, K = 5;
        const size_t N = 200, K = 9;
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
        genesis.nTime = 1531350314;
        genesis.nBits = 0x2007ffff;
        genesis.nNonce = uint256S("0x0000000000000000000000000000000000000000000000000000000000000003");
        genesis.nSolution = ParseHex("0152b04ab791857d06182362176679efc7937ed3c4116a8ce3bf1d66750c50336048cc842f72591e94181a3eab9020b4dc01e16942cfe2c4e079847b75ad6e269617e2f5d2a12547d8c621d0cbce9e02605a0a3403f9527b798255794efcd2b36ae707fe62dbde55ff04c961ec1b498fe2d374a47d0646fe95cdf3ba85ef0ff740be0e143a476b36b374f4765031799c3b086914acadb84a51b9cbe7a3a42af03df83d481075f54502b34777ba430bb97253c0af8ba1f0114180706f762b7ec9f7cd9d2aa7f5f2f3cb2ac31f17072f789a3742819e40d8e4ab357dfbd50873d02c0599c9d5706f591423e97e9f391b11b72a24fc5d1f2b2f6499bd8f108f876996c7011385b7015983d7262b28f89b2a5b1f38b6e88dc9297b55b7448216787c19bead0f79ec17f5aae4e9d4454ed398c17ffbacb03ce4f6753fbe452cfd174f556b38eed7b4bcdfb0238990cbf4b377038cc88db3d3ab2eb397c77859646472459d971c331671bad3572892df89c552d5541b73c2267d3ccc520ff08d3a63ccf846955e03898e4dccef56d41ae4aa251e19a8221e0cadad4e83942cfdd74a3be1bcc6b70778758b5d93cdfd4e0a808781ffe4883b446b522c1232af94c2e4273fce08a34231ff7ab9fecafe3adf0a88173189975b7cd9fb03693342929ec28dff0cf668da3f51bba8f4b19b6426b686f9e6fe10b8f58bfa12d115edd3a6df539f7851f02c9379ec7d5759fb0519a04a82495b9cb7294d68439355b3f32d49ffa8fc175561620811bf89a6f1131ec16af141e04fb788202d722c589297bbaf751f847d51c7aa66626f15cc0715d7efc138575771c5ed07d9a37838ee13b03380f2237b5f606115c8ed6eb7837f4af0a38962edf78387182bbe0664d107097666b78c42c6a296e04539d6d44d9cd4a628726445b9a2b532594d22269fc91cb19e0249ad8dd01d12876c517246c3655fa57c0bf869140cfc754e4c84234af466435879a6934cdb6c324d5e026a2e67c9b44a73baca64d189f308e676147c7f19166da2e4db9622aac0d8a4077a73b6a736867f59df08fcb0511e0343851fc38415a8ab7e130d6ffa255d6fb944bed45fca0f86cc28db84e883bef7d3dfc3f7170484f96037dc9bf53c63a0ce7f3113499e9cb816241ad3ff16184cf3d810a2d2602ddf62102d7c05cb0abd89f5746465e1c523a76ff83c330e9a24b74d72290681918e99d03cf6f474736e6ada39d4833c604113087517dc6ad4ad6c3855ce47cc28168f1b7748fb1dcf0ee1e5c9df850c8c867b0c45bbf6468396a681116807f45311f5fcdd67327cdfde4815b3659853805b3836d0b95cfc5d407d08729ec66e727c109fda5c1824242eb0a5a4ab8c2fd4bd39b813f3765c9cdab526294c228ebb9323ebba472da0c3135abe3937d70f02a7b4b414cf64b2a0cd70b4e5d8879503dd7692501af91e3773ec6a25a77aa1cfa36ff701782c97a4140546003ff10f787910bfb19d9b17a9c20258fd11c9203c517b460f708efc0c33f659d5c02e111ebb16990a5e73e5ec635e8d8e8574a361574b8e331bdb8a7a0c849152e30f09535f510550c4514621f563d4c00f14796f483e1360a3f170e1bab9d495e973ddcf46d03169a6b57efa3897e8bf24426de755f97792bf7003045a2e0cb3ecf613cfc191df0344421b75cbbc407e0a75e1dfe9bb8b4de147ac9167794437976b7da26608898148120c5363439111d2fb2df8e8d4e50d196b1642d5ffdbe83c5f796891d7a7f8d9e16f9479b175076a535c39640fa1a07fb5b0377629862fdd92fba7090fb905020a8a20804012da9d6a998918a60e4e9b0859f96966119decab0000f30ef22a4216b6bc58f01e43dc2f3a0e211de07d6302f5724bb24c0a5c2050");
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x064c5de072fb7106f34f1a0d2f1ef11aecdb2f140ab6dbbf24d93a5124402e6f"));

        vFixedSeeds.clear();
        vSeeds.clear();

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

        nForkStartHeight = 7;
        nForkHeightRange = 1724;
        zUtxoMiningStartBlock = 8;
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




