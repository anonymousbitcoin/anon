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
<<<<<<< HEAD
        nDefaultPort = 9695;
=======
        nDefaultPort = 33130;
>>>>>>> 343b685f9... Merge conflicts
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
        // assert(genesis.hashMerkleRoot == uint256S("0x19612bcf00ea7611d315d7f43554fa983c6e8c30cba17e52c679e0e80abf7d42"));

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

        // fMiningRequiresPeers = true;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        checkpointData = {
            {
<<<<<<< HEAD
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
=======
                {      0, consensus.hashGenesisBlock },
            },
            1520105951,     // * UNIX timestamp of last checkpoint block
>>>>>>> 343b685f9... Merge conflicts
            0,       // * total number of transactions between genesis and last checkpoint
                            //   (the tx=... number in the SetBestChain debug.log lines)
            50            // * estimated number of transactions per day after checkpoint
                            //   total number of tx / (checkpoint block height / (24 * 24))
        };

<<<<<<< HEAD
        nForkStartHeight = 0;
        nForkHeightRange = 0;
=======
        nForkStartHeight = 5;
        nForkHeightRange = 1000;
>>>>>>> 343b685f9... Merge conflicts
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
        nDefaultPort = 12346;
        nMaxTipAge = 0x7fffffff;
        nPruneAfterHeight = 1000;


        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1531341893;
        genesis.nBits = 0x2007ffff;
        genesis.nNonce = uint256S("0x0000000000000000000000000000000000000000000000000000000000000003");
        genesis.nSolution = ParseHex("000bb147a15c27ed9367d6c2b13cf3ff5fdf3b4db00580271d76c663cd5f96171364583982376434988d11c68c977aca5d326e83c508972fd047e9f73f5a432a1bcb9561987979811966c2d964893650dff56e0109c8fe9af283a1fddabbc1e56fc6ffc21b7cd9c69b15e71662aee621dbe8e971a55ed2802195051f59ee09f3f06dd711f512d56e22299bc96a2909aff9550d0f711755edd7649ee3a4e7b860ef76e3617b3cb78f039ffa3ea1919e5f466af75b286df242f05a9b3a4520a89dfb26a35a6b2eef4355ac4dfd05aeaf94e06106caf8bdf2588b2935470152ccdacb01edb2f39add0777adbe9e87b9ad530d309f5a961ef1a76ab8d9a11527265710d6670eb52a631eb7b5b24d866a753eda20bbffb91f96491d789a57afa1684eff27441d4299179500d66f8c512bedc04258f7585480caf4335ea51805459891dcd7af458b2278c2e8440297cb1c8cd90175f272f2331fb7bbf704db16fe205d5cb7ae68c801a5d5c04104f7aef551157debc801edd0413097a0031689716163e0694e84b79f8140f09a291718441117727fe2c4c9d9b68d7e9a745a6de61366ad3b821e04136bd23b9e5195a6ff020d36a89e0562538d8d5512ec1b47bae707314245c3f2edd0b8312059fcfecf1d5d27a02c894364890696f0677c970a8fa1bcfda1274f2fe7ab37c229cbe5a730775cc0b5e62b90e56b0654619d3fd9b45af4e2e2a8711cc789efaf2fa0f70d1ca6c42aa2dc6b3b3e550772b8b526756c1f1d320a3c168ede09774fe85a24a701bb5efe5e0e1396b90c5297fa1f085340acd8c269c99f8f20a55b313421080ed3d91f58b5dacbc8e4ec32d3fb9ef526f982241b2ca7f7bf70bc8bcd8eb259f92e4a352c825a5c41187ba1e2b4e5297b37b481e923dc72e88c9c55d37c29840ae28094c31b29d10361b12441251a1c990a670020ca2cfc00371b8165d47163fe05da425375aac91d04ad8d25abc94166eaeb62b876b4daf88fb9c45303db0a8d17c228ce91b76525a148555a68f39d2cf30d0303df0c1507b14e60c41bc7d32291617ebcae9a09c7d8b6b149c03e5f7e621df4d0da81f53b1850c520dc4413fe14fb979a16c396b133fd92d3e818ce3e213f448b79dece7d8975834c842ed3c59bab972aa1455917cc2f52fadd6c2ed56883d3f805e8497637bb09582ab8a11cc42d3b16d69aa337c8529aa21802550b55e5876fe5a797d1f3e2d1485fe5836f825f8e8d1b434ad14557c992bff088d1f8cd03529459f769453ff623cfcb953e19cd36860a1373bb761504b495e015e7ccbb5b1339d3783dc30c1bb606364793fef3b3313896ccc066348fbf17ca86d4562dfedb049c8a5f1d98531530e21acbc305029a4cf84244cf567d70493adea4ef2696874ac967d5f5aeee9909f1bc16b0610053978a6850791f7720a161ec2c6384bb37e6caaa12acbff8e385ef1c81235379a34dee5535f3df941805ca376b0b03fcedbdcac511b13ba31777661d85651ee25a676afa3c49ddbbc80080fa488fe0e5bfee6301e6194b58d09194d47551ada8cc050169642ce5b038d4622c38580b0da36314c5e76cbcb15ac86fd648101871c8bac9415fd585f72fb148994a0f5a70d1c710d52119da858fe1d734f2f6c0486b61583a50ebe40167645fa9d36356cbc1a28ef31645ce26047b7db8314ed9ffc7d76e6bf20d536feccceaa55423cf26260257374e2ce411e9d3b541226de5625971397679f921694563c7cb2e578620130dc8e9b8adf5e0d769b6039c1db2e8ce2f42ff26c11a83bbd4c85c9d962f2b086824f916c6f20187b9a15c06368e455906f10b261a96794389f908e7cb76828bf05c3da9ec527922dd8bf605dde032cd0be1063909f957fc06fa4e188333");
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x070deca88aa9e05493013e4d9cc03e9c1ba6d51eeb241897d1b063d732a7bc07"));

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

