// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CHAINPARAMS_H
#define BITCOIN_CHAINPARAMS_H

#include "chainparamsbase.h"
#include "checkpoints.h"
#include "consensus/params.h"
#include "primitives/block.h"
#include "protocol.h"

#include <vector>

struct CDNSSeedData {
    std::string name, host;
    CDNSSeedData(const std::string &strName, const std::string &strHost) : name(strName), host(strHost) {}
};

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

struct EHparameters {
    unsigned char n;
    unsigned char k;
    unsigned short int nSolSize;
};

//EH sol size = (pow(2, k) * ((n/(k+1))+1)) / 8;
static const EHparameters eh200_9 = {200,9,1344};
static const EHparameters eh192_7 = {192,7,400};
static const EHparameters eh144_5 = {144,5,100};
static const EHparameters eh96_5 = {96,5,68};
static const EHparameters eh48_5 = {48,5,36};
static const unsigned int MAX_EH_PARAM_LIST_LEN = 2;


/**
 * CChainParams defines various tweakable parameters of a given instance of the
 * Bitcoin system. There are three: the main network on which people trade goods
 * and services, the public test network which gets reset from time to time and
 * a regression test mode which is intended for private networks only. It has
 * minimal difficulty to ensure that blocks can be found instantly.
 */
class CChainParams
{
public:
    enum Base58Type {
        PUBKEY_ADDRESS,
        SCRIPT_ADDRESS,
        SECRET_KEY,
        EXT_PUBLIC_KEY,
        EXT_SECRET_KEY,

        ZCPAYMENT_ADDRRESS,
        ZCSPENDING_KEY,
        ZCVIEWING_KEY,

        MAX_BASE58_TYPES
    };

    const Consensus::Params& GetConsensus() const { return consensus; }
    const CMessageHeader::MessageStartChars& MessageStart() const { return pchMessageStart; }
    const std::vector<unsigned char>& AlertKey() const { return vAlertPubKey; }
    int GetDefaultPort() const { return nDefaultPort; }

    const CBlock& GenesisBlock() const { return genesis; }
    /** Make miner wait to have peers to avoid wasting work */
    bool MiningRequiresPeers() const { return fMiningRequiresPeers; }
    /** Default value for -checkmempool and -checkblockindex argument */
    bool DefaultConsistencyChecks() const { return fDefaultConsistencyChecks; }
    /** Policy: Filter transactions that do not match well-defined patterns */
    bool RequireStandard() const { return fRequireStandard; }
    int64_t MaxTipAge() const { return nMaxTipAge; }
    int64_t PruneAfterHeight() const { return nPruneAfterHeight; }
    unsigned int EquihashN() const { return nEquihashN; }
    unsigned int EquihashK() const { return nEquihashK; }

    EHparameters eh_epoch_1_params() const { return eh_epoch_1; }
    EHparameters eh_epoch_2_params() const { return eh_epoch_2; }
    EHparameters eh_epoch_3_params() const { return eh_epoch_3; }
    unsigned long eh_epoch_1_end() const { return eh_epoch_1_endblock; }
    unsigned long eh_epoch_2_start() const { return eh_epoch_2_startblock; }
    unsigned long eh_epoch_3_start() const { return eh_epoch_3_startblock; }

    std::string CurrencyUnits() const { return strCurrencyUnits; }
    /** Make miner stop after a block is found. In RPC, don't return until nGenProcLimit blocks are generated */
    bool MineBlocksOnDemand() const { return fMineBlocksOnDemand; }
    /** In the future use NetworkIDString() for RPC fields */
    bool TestnetToBeDeprecatedFieldRPC() const { return fTestnetToBeDeprecatedFieldRPC; }
    /** Return the BIP70 network string (main, test or regtest) */
    std::string NetworkIDString() const { return strNetworkID; }
    const std::vector<CDNSSeedData>& DNSSeeds() const { return vSeeds; }
    const std::vector<unsigned char>& Base58Prefix(Base58Type type) const { return base58Prefixes[type]; }
    const std::vector<SeedSpec6>& FixedSeeds() const { return vFixedSeeds; }
    const Checkpoints::CCheckpointData& Checkpoints() const { return checkpointData; }
    /** Enforce coinbase consensus rule in regtest mode */
    void SetRegTestCoinbaseMustBeProtected() { consensus.fCoinbaseMustBeProtected = true; }
    int FulfilledRequestExpireTime() const { return nFulfilledRequestExpireTime; }

    //Sporks
    std::string SporkPubKey() const { return strSporkPubKey; }

    uint64_t ForkStartHeight() const { return nForkStartHeight; };
    uint64_t ForkHeightRange() const { return nForkHeightRange; };
    uint64_t ZshieldedStartBlock() const { return nZshieldedStartBlock; };
    uint64_t ZtransparentStartBlock() const { return nZtransparentStartBlock; };

    /** Return the founder's reward address and script for a given block height */
    std::string GetFoundersRewardAddressAtHeight(int height) const;
    CScript GetFoundersRewardScriptAtHeight(int height) const;
    std::string GetFoundersRewardAddressAtIndex(int i) const;
    int GetMasternodeCollateral(int nHeight) const;
    bool isGrothActive(int nHeight) const;

    int GetFoundersRewardBlockStart() const { return nFoundersRewardBlockStart; };
    int GetNewTimeRule() const { return newTimeRule; }
    uint64_t LwmaAveragingWin() const { return lwmaAveragingWindow; };


protected:
    CChainParams() {}

    Consensus::Params consensus;
    CMessageHeader::MessageStartChars pchMessageStart;
    //! Raw pub key bytes for the broadcast alert signing key.
    std::vector<unsigned char> vAlertPubKey;
    int nDefaultPort = 0;
    long nMaxTipAge = 0;
    uint64_t nPruneAfterHeight = 0;

    EHparameters eh_epoch_1 = eh200_9;
    EHparameters eh_epoch_2 = eh144_5;
    EHparameters eh_epoch_3 = eh192_7;
    unsigned long eh_epoch_1_endblock = 5;
    unsigned long eh_epoch_2_startblock = 10;
    unsigned long eh_epoch_3_startblock = 20;

    unsigned int nEquihashN = 0;
    unsigned int nEquihashK = 0;
    std::vector<CDNSSeedData> vSeeds;
    std::vector<unsigned char> base58Prefixes[MAX_BASE58_TYPES];
    std::string strNetworkID;
    std::string strCurrencyUnits;
    CBlock genesis;
    std::vector<SeedSpec6> vFixedSeeds;
    bool fMiningRequiresPeers = false;
    bool fDefaultConsistencyChecks = false;
    bool fRequireStandard = false;
    bool fMineBlocksOnDemand = false;
    bool fTestnetToBeDeprecatedFieldRPC = false;
    Checkpoints::CCheckpointData checkpointData;
    int nFulfilledRequestExpireTime;
    std::vector<std::string> vFoundersRewardAddress;
    std::vector<std::string> vFoundersRewardAddress2;

    //Sporks
    std::string strSporkPubKey;

    uint64_t nForkStartHeight;
    uint64_t nForkHeightRange;
    uint64_t nZshieldedStartBlock;
    uint64_t nZtransparentStartBlock;

    //Masternode related
    int masternodeCollateralChangeBlock;
    int masternodeCollateralOld;
    int masternodeCollateralNew;

    //Sapling 
    int saplingActivationBlock;

    //Founders reward
    int nFoundersRewardBlockStart;
    int foundersRewardAddressPeriod;

    //diff
    int newTimeRule;

    // lwma
    int64_t lwmaAveragingWindow;

};

/**
 * Return the currently selected parameters. This won't change after app
 * startup, except for unit tests.
 */
const CChainParams &Params();

/** Return parameters for the given network. */
CChainParams &Params(CBaseChainParams::Network network);

/** Sets the params returned by Params() to those for the given network. */
void SelectParams(CBaseChainParams::Network network);

/**
 * Looks for -regtest or -testnet and then calls SelectParams as appropriate.
 * Returns false if an invalid combination is given.
 */
bool SelectParamsFromCommandLine();

int validEHparameterList(EHparameters *ehparams, unsigned long blockheight, const CChainParams& params);

#endif // BITCOIN_CHAINPARAMS_H
