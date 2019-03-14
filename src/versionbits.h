// Copyright (c) 2016-2018 The dogxcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef dogxCOIN_VERSIONdogxS_H
#define dogxCOIN_VERSIONdogxS_H

#include <chain.h>
#include <map>

/** What block version to use for new blocks (pre versiondogxs) */
static const int32_t VERSIONdogxS_LAST_OLD_BLOCK_VERSION = 4;
/** What dogxs to set in version for versiondogxs blocks */
static const int32_t VERSIONdogxS_TOP_dogxS = 0x20000000UL;
/** What dogxmask determines whether versiondogxs is in use */
static const int32_t VERSIONdogxS_TOP_MASK = 0xE0000000UL;
/** Total dogxs available for versiondogxs */
static const int32_t VERSIONdogxS_NUM_dogxS = 29;

enum class ThresholdState {
    DEFINED,
    STARTED,
    LOCKED_IN,
    ACTIVE,
    FAILED,
};

// A map that gives the state for blocks whose height is a multiple of Period().
// The map is indexed by the block's parent, however, so all keys in the map
// will either be nullptr or a block with (height + 1) % Period() == 0.
typedef std::map<const CBlockIndex*, ThresholdState> ThresholdConditionCache;

struct BIP9Stats {
    int period;
    int threshold;
    int elapsed;
    int count;
    bool possible;
};

/**
 * Abstract class that implements BIP9-style threshold logic, and caches results.
 */
class AbstractThresholdConditionChecker {
protected:
    virtual bool Condition(const CBlockIndex* pindex, const Consensus::Params& params) const =0;
    virtual int64_t BeginTime(const Consensus::Params& params) const =0;
    virtual int64_t EndTime(const Consensus::Params& params) const =0;
    virtual int Period(const Consensus::Params& params) const =0;
    virtual int Threshold(const Consensus::Params& params) const =0;

public:
    BIP9Stats GetStateStatisticsFor(const CBlockIndex* pindex, const Consensus::Params& params) const;
    // Note that the functions below take a pindexPrev as input: they compute information for block B based on its parent.
    ThresholdState GetStateFor(const CBlockIndex* pindexPrev, const Consensus::Params& params, ThresholdConditionCache& cache) const;
    int GetStateSinceHeightFor(const CBlockIndex* pindexPrev, const Consensus::Params& params, ThresholdConditionCache& cache) const;
};

struct VersiondogxsCache
{
    ThresholdConditionCache caches[Consensus::MAX_VERSION_dogxS_DEPLOYMENTS];

    void Clear();
};

ThresholdState VersiondogxsState(const CBlockIndex* pindexPrev, const Consensus::Params& params, Consensus::DeploymentPos pos, VersiondogxsCache& cache);
BIP9Stats VersiondogxsStatistics(const CBlockIndex* pindexPrev, const Consensus::Params& params, Consensus::DeploymentPos pos);
int VersiondogxsStateSinceHeight(const CBlockIndex* pindexPrev, const Consensus::Params& params, Consensus::DeploymentPos pos, VersiondogxsCache& cache);
uint32_t VersiondogxsMask(const Consensus::Params& params, Consensus::DeploymentPos pos);

#endif // dogxCOIN_VERSIONdogxS_H
