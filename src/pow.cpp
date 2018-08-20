// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "chainparams.h"
#include "crypto/equihash.h"
#include "main.h"
#include "primitives/block.h"
#include "streams.h"
#include "uint256.h"
#include "util.h"

#include "sodium.h"

#ifdef ENABLE_RUST
#include "librustzcash.h"
#endif // ENABLE_RUST


unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    int nHeight = pindexLast->nHeight + 1;

    arith_uint256 proofOfWorkLimit;
    if(!isForkEnabled(nHeight))
        proofOfWorkLimit = UintToArith256(params.prePowLimit);
    else
        proofOfWorkLimit = UintToArith256(params.powLimit);

    unsigned int nProofOfWorkLimit = proofOfWorkLimit.GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // right at fork
    else if(isForkBlock(nHeight))
        return nProofOfWorkLimit;

    // right post fork
    else if(!isForkBlock(nHeight) && isForkBlock(nHeight - params.nPowAveragingWindow))
        return nProofOfWorkLimit;

    // Find the first block in the averaging interval
    const CBlockIndex* pindexFirst = pindexLast;
    arith_uint256 bnTot {0};
    for (int i = 0; pindexFirst && i < params.nPowAveragingWindow; i++) {
        arith_uint256 bnTmp;
        bnTmp.SetCompact(pindexFirst->nBits);
        bnTot += bnTmp;
        pindexFirst = pindexFirst->pprev;
    }

    // Check we have enough blocks
    if (pindexFirst == NULL)
        return nProofOfWorkLimit;

    arith_uint256 bnAvg {bnTot / params.nPowAveragingWindow};

    bool isFork = isForkBlock(pindexLast->nHeight + 1);
    return CalculateNextWorkRequired(bnAvg, pindexLast->GetMedianTimePast(), pindexFirst->GetMedianTimePast(), params, proofOfWorkLimit, isFork);
}

unsigned int CalculateNextWorkRequired(arith_uint256 bnAvg,
                                       int64_t nLastBlockTime, int64_t nFirstBlockTime,
                                       const Consensus::Params& params, const arith_uint256 bnPowLimit, bool isFork)
{
    // Limit adjustment step
    // Use medians to prevent time-warp attacks
    int64_t nActualTimespan = nLastBlockTime - nFirstBlockTime;
    LogPrint("pow", "  nActualTimespan = %d  before dampening\n", nActualTimespan);
    nActualTimespan = params.AveragingWindowTimespan(isFork) + (nActualTimespan - params.AveragingWindowTimespan(isFork))/4;
    LogPrint("pow", "  nActualTimespan = %d  before bounds\n", nActualTimespan);

    if (nActualTimespan < params.MinActualTimespan(isFork))
        nActualTimespan = params.MinActualTimespan(isFork);
    if (nActualTimespan > params.MaxActualTimespan(isFork))
        nActualTimespan = params.MaxActualTimespan(isFork);

    // Retarget
    arith_uint256 bnNew {bnAvg};
    bnNew /= params.AveragingWindowTimespan(isFork);
    bnNew *= nActualTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    /// debug print
    LogPrint("pow", "GetNextWorkRequired RETARGET\n");
    LogPrint("pow", "params.AveragingWindowTimespan() = %d    nActualTimespan = %d\n", params.AveragingWindowTimespan(), nActualTimespan);
    LogPrint("pow", "Current average: %08x  %s\n", bnAvg.GetCompact(), bnAvg.ToString());
    LogPrint("pow", "After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());

    return bnNew.GetCompact();
}



// unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
// {
//     int nHeight = pindexLast->nHeight + 1;

//     unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

//     // Genesis block ???
//     if (pindexLast == NULL)
//         return nProofOfWorkLimit;

//     // right at fork
//     else if(isForkBlock(nHeight))
//         return nProofOfWorkLimit;

//     int64_t DifficultyAdjustmentInterval =  params.nPowAveragingWindow;

//       if (nHeight % DifficultyAdjustmentInterval != 0)      
//     {
//         LogPrintf("Inside nHeight % DifficultyAdjustmentInterval != 0\n");
//         if (params.fPowAllowMinDifficultyBlocks)
//         {
//             // Special difficulty rule for testnet:
//             // If the new block's timestamp is more than 2* 10 minutes
//             // then allow mining of a min-difficulty block.
//             if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
//                 return nProofOfWorkLimit;
//             else
//             {
//                 // Return the last non-special-min-difficulty-rules-block
//                 const CBlockIndex* pindex = pindexLast;
//                 while (pindex->pprev && pindex->nHeight % DifficultyAdjustmentInterval != 0 && pindex->nBits == nProofOfWorkLimit)
//                     pindex = pindex->pprev;
//                 return pindex->nBits;
//             }
//         }
//         return pindexLast->nBits;
//     }

//     // Go back by what we want to be 14 days worth of blocks
//     int nHeightFirst = pindexLast->nHeight - (DifficultyAdjustmentInterval-1);
//     assert(nHeightFirst >= 0);
//     const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
//     assert(pindexFirst);
//     LogPrintf("Before return CalculateNextWorkRequired\n");
//     return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
//     //////////////////////////////////////////////////////////

// }

// unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
// {   
//     //for regtest
//     if (params.fPowNoRetargeting)
//         return pindexLast->nBits;

//     // Limit adjustment step
//     LogPrintf("nFirstBlockTime: %d \n", nFirstBlockTime);
//     LogPrintf("pindexLast->GetBlockTime(): %d \n", pindexLast->GetBlockTime());
//     int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
//     LogPrintf("nActualTimespan: %d \n", nActualTimespan);
//     LogPrintf("params.nPowTargetTimespan/4: %d \n", params.nPowTargetTimespan/4);
//     LogPrintf("params.nPowTargetTimespan*4: %d \n", params.nPowTargetTimespan*4);
//     if (nActualTimespan < params.nPowTargetTimespan/4){
//         LogPrintf("1\n");
//         nActualTimespan = params.nPowTargetTimespan/4;
//     }
//     if (nActualTimespan > params.nPowTargetTimespan*4){
//         LogPrintf("2\n");
//         nActualTimespan = params.nPowTargetTimespan*4;
//     }

//     // Retarget
//     LogPrintf("powLimit: %s\n", params.powLimit.ToString());
//     LogPrintf("nPowTargetTimespan: %d\n", params.nPowTargetTimespan);
//     const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
//     arith_uint256 bnNew;
//     bnNew.SetCompact(pindexLast->nBits);
//     LogPrintf("bnNew after initialization with bits of last block: %d\n", bnNew.GetCompact());
//     LogPrintf("Actual timespan over the last 2016 blocks: %d\n", nActualTimespan);
//     // bnNew *= nActualTimespan;
//     bnNew /= 4;
//     LogPrintf("bnNew after multiplication with actual timespan: %s\n", bnNew.ToString());
//     // bnNew /= params.nPowTargetTimespan;
//     LogPrintf("bnNew after division by target timespan: %s\n", bnNew.ToString());

//     if (bnNew > bnPowLimit) {
//         bnNew = bnPowLimit;
//         LogPrintf("Went inside if(bnNew > bnPowLimit)\n");
//         }
//     LogPrintf("bnNew: %s\n", bnNew.ToString());
//     LogPrintf("3\n");
//     return bnNew.GetCompact();
// }

// unsigned int CalculateNextWorkRequired(arith_uint256 bnAvg,
//                                        int64_t nLastBlockTime, int64_t nFirstBlockTime,
//                                        const Consensus::Params& params, const arith_uint256 bnPowLimit, bool isFork)
// {
//     // Limit adjustment step
//     // Use medians to prevent time-warp attacks
//     int64_t nActualTimespan = nLastBlockTime - nFirstBlockTime;
//     LogPrint("pow", "  nActualTimespan = %d  before dampening\n", nActualTimespan);
//     nActualTimespan = params.AveragingWindowTimespan(isFork) + (nActualTimespan - params.AveragingWindowTimespan(isFork))/4;
//     LogPrint("pow", "  nActualTimespan = %d  before bounds\n", nActualTimespan);

//     if (nActualTimespan < params.MinActualTimespan(isFork))
//         nActualTimespan = params.MinActualTimespan(isFork);
//     if (nActualTimespan > params.MaxActualTimespan(isFork))
//         nActualTimespan = params.MaxActualTimespan(isFork);

//     // Retarget
//     arith_uint256 bnNew {bnAvg};
//     bnNew /= params.AveragingWindowTimespan(isFork);
//     bnNew *= nActualTimespan;

//     if (bnNew > bnPowLimit)
//         bnNew = bnPowLimit;

//     /// debug print
//     LogPrint("pow", "GetNextWorkRequired RETARGET\n");
//     LogPrint("pow", "params.AveragingWindowTimespan() = %d    nActualTimespan = %d\n", params.AveragingWindowTimespan(), nActualTimespan);
//     LogPrint("pow", "Current average: %08x  %s\n", bnAvg.GetCompact(), bnAvg.ToString());
//     LogPrint("pow", "After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());

//     return bnNew.GetCompact();
// }

bool CheckEquihashSolution(const CBlockHeader *pblock, const CChainParams& params)
{
    unsigned int n = params.EquihashN();
    unsigned int k = params.EquihashK();

    size_t nSolSize = pblock->nSolution.size();
    switch (nSolSize){
        case 1344: n=200; k=9; break;
        case 100:  n=144; k=5; break;
        case 68:   n=96;  k=5; break;
        case 36:   n=48;  k=5; break;
        default: return error("CheckEquihashSolution: Unsupported solution size of %d", nSolSize);
    }

    // Hash state
    crypto_generichash_blake2b_state state;
    EhInitialiseState(n, k, state);

    // I = the block header minus nonce and solution.
    CEquihashInput I{*pblock};
    // I||V
    CDataStream ss(SER_NETWORK, PROTOCOL_VERSION);
    ss << I;
    ss << pblock->nNonce;

    // H(I||V||...
    crypto_generichash_blake2b_update(&state, (unsigned char*)&ss[0], ss.size());

    #ifdef ENABLE_RUST
    // Ensure that our Rust interactions are working in production builds. This is
    // temporary and should be removed.
    {
        assert(librustzcash_xor(0x0f0f0f0f0f0f0f0f, 0x1111111111111111) == 0x1e1e1e1e1e1e1e1e);
    }
    #endif // ENABLE_RUST

    bool isValid;
    EhIsValidSolution(n, k, state, pblock->nSolution, isValid);
    if (!isValid)
        return error("CheckEquihashSolution(): invalid solution");

    return true;
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return error("CheckProofOfWork(): nBits below minimum work");

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return error("CheckProofOfWork(): hash doesn't match nBits");

    return true;
}

arith_uint256 GetBlockProof(const CBlockIndex& block)
{
    arith_uint256 bnTarget;
    bool fNegative;
    bool fOverflow;
    bnTarget.SetCompact(block.nBits, &fNegative, &fOverflow);
    if (fNegative || fOverflow || bnTarget == 0)
        return 0;
    // We need to compute 2**256 / (bnTarget+1), but we can't represent 2**256
    // as it's too large for a arith_uint256. However, as 2**256 is at least as large
    // as bnTarget+1, it is equal to ((2**256 - bnTarget - 1) / (bnTarget+1)) + 1,
    // or ~bnTarget / (nTarget+1) + 1.
    return (~bnTarget / (bnTarget + 1)) + 1;
}

int64_t GetBlockProofEquivalentTime(const CBlockIndex& to, const CBlockIndex& from, const CBlockIndex& tip, const Consensus::Params& params)
{
    arith_uint256 r;
    int sign = 1;
    if (to.nChainWork > from.nChainWork) {
        r = to.nChainWork - from.nChainWork;
    } else {
        r = from.nChainWork - to.nChainWork;
        sign = -1;
    }
    r = r * arith_uint256(params.nPowTargetSpacing) / GetBlockProof(tip);
    if (r.bits() > 63) {
        return sign * std::numeric_limits<int64_t>::max();
    }
    return sign * r.GetLow64();
}
