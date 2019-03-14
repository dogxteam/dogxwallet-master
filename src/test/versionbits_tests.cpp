// Copyright (c) 2014-2018 The dogxcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chain.h>
#include <versiondogxs.h>
#include <test/test_dogxcoin.h>
#include <chainparams.h>
#include <validation.h>
#include <consensus/params.h>

#include <boost/test/unit_test.hpp>

/* Define a virtual block time, one block per 10 minutes after Nov 14 2014, 0:55:36am */
static int32_t TestTime(int nHeight) { return 1415926536 + 600 * nHeight; }

static const Consensus::Params paramsDummy = Consensus::Params();

class TestConditionChecker : public AbstractThresholdConditionChecker
{
private:
    mutable ThresholdConditionCache cache;

public:
    int64_t BeginTime(const Consensus::Params& params) const override { return TestTime(10000); }
    int64_t EndTime(const Consensus::Params& params) const override { return TestTime(20000); }
    int Period(const Consensus::Params& params) const override { return 1000; }
    int Threshold(const Consensus::Params& params) const override { return 900; }
    bool Condition(const CBlockIndex* pindex, const Consensus::Params& params) const override { return (pindex->nVersion & 0x100); }

    ThresholdState GetStateFor(const CBlockIndex* pindexPrev) const { return AbstractThresholdConditionChecker::GetStateFor(pindexPrev, paramsDummy, cache); }
    int GetStateSinceHeightFor(const CBlockIndex* pindexPrev) const { return AbstractThresholdConditionChecker::GetStateSinceHeightFor(pindexPrev, paramsDummy, cache); }
};

class TestAlwaysActiveConditionChecker : public TestConditionChecker
{
public:
    int64_t BeginTime(const Consensus::Params& params) const override { return Consensus::BIP9Deployment::ALWAYS_ACTIVE; }
};

#define CHECKERS 6

class VersiondogxsTester
{
    // A fake blockchain
    std::vector<CBlockIndex*> vpblock;

    // 6 independent checkers for the same dogx.
    // The first one performs all checks, the second only 50%, the third only 25%, etc...
    // This is to test whether lack of cached information leads to the same results.
    TestConditionChecker checker[CHECKERS];
    // Another 6 that assume always active activation
    TestAlwaysActiveConditionChecker checker_always[CHECKERS];

    // Test counter (to identify failures)
    int num;

public:
    VersiondogxsTester() : num(0) {}

    VersiondogxsTester& Reset() {
        for (unsigned int i = 0; i < vpblock.size(); i++) {
            delete vpblock[i];
        }
        for (unsigned int  i = 0; i < CHECKERS; i++) {
            checker[i] = TestConditionChecker();
            checker_always[i] = TestAlwaysActiveConditionChecker();
        }
        vpblock.clear();
        return *this;
    }

    ~VersiondogxsTester() {
         Reset();
    }

    VersiondogxsTester& Mine(unsigned int height, int32_t nTime, int32_t nVersion) {
        while (vpblock.size() < height) {
            CBlockIndex* pindex = new CBlockIndex();
            pindex->nHeight = vpblock.size();
            pindex->pprev = vpblock.size() > 0 ? vpblock.back() : nullptr;
            pindex->nTime = nTime;
            pindex->nVersion = nVersion;
            pindex->BuildSkip();
            vpblock.push_back(pindex);
        }
        return *this;
    }

    VersiondogxsTester& TestStateSinceHeight(int height) {
        for (int i = 0; i < CHECKERS; i++) {
            if (InsecureRanddogxs(i) == 0) {
                BOOST_CHECK_MESSAGE(checker[i].GetStateSinceHeightFor(vpblock.empty() ? nullptr : vpblock.back()) == height, strprintf("Test %i for StateSinceHeight", num));
                BOOST_CHECK_MESSAGE(checker_always[i].GetStateSinceHeightFor(vpblock.empty() ? nullptr : vpblock.back()) == 0, strprintf("Test %i for StateSinceHeight (always active)", num));
            }
        }
        num++;
        return *this;
    }

    VersiondogxsTester& TestDefined() {
        for (int i = 0; i < CHECKERS; i++) {
            if (InsecureRanddogxs(i) == 0) {
                BOOST_CHECK_MESSAGE(checker[i].GetStateFor(vpblock.empty() ? nullptr : vpblock.back()) == ThresholdState::DEFINED, strprintf("Test %i for DEFINED", num));
                BOOST_CHECK_MESSAGE(checker_always[i].GetStateFor(vpblock.empty() ? nullptr : vpblock.back()) == ThresholdState::ACTIVE, strprintf("Test %i for ACTIVE (always active)", num));
            }
        }
        num++;
        return *this;
    }

    VersiondogxsTester& TestStarted() {
        for (int i = 0; i < CHECKERS; i++) {
            if (InsecureRanddogxs(i) == 0) {
                BOOST_CHECK_MESSAGE(checker[i].GetStateFor(vpblock.empty() ? nullptr : vpblock.back()) == ThresholdState::STARTED, strprintf("Test %i for STARTED", num));
                BOOST_CHECK_MESSAGE(checker_always[i].GetStateFor(vpblock.empty() ? nullptr : vpblock.back()) == ThresholdState::ACTIVE, strprintf("Test %i for ACTIVE (always active)", num));
            }
        }
        num++;
        return *this;
    }

    VersiondogxsTester& TestLockedIn() {
        for (int i = 0; i < CHECKERS; i++) {
            if (InsecureRanddogxs(i) == 0) {
                BOOST_CHECK_MESSAGE(checker[i].GetStateFor(vpblock.empty() ? nullptr : vpblock.back()) == ThresholdState::LOCKED_IN, strprintf("Test %i for LOCKED_IN", num));
                BOOST_CHECK_MESSAGE(checker_always[i].GetStateFor(vpblock.empty() ? nullptr : vpblock.back()) == ThresholdState::ACTIVE, strprintf("Test %i for ACTIVE (always active)", num));
            }
        }
        num++;
        return *this;
    }

    VersiondogxsTester& TestActive() {
        for (int i = 0; i < CHECKERS; i++) {
            if (InsecureRanddogxs(i) == 0) {
                BOOST_CHECK_MESSAGE(checker[i].GetStateFor(vpblock.empty() ? nullptr : vpblock.back()) == ThresholdState::ACTIVE, strprintf("Test %i for ACTIVE", num));
                BOOST_CHECK_MESSAGE(checker_always[i].GetStateFor(vpblock.empty() ? nullptr : vpblock.back()) == ThresholdState::ACTIVE, strprintf("Test %i for ACTIVE (always active)", num));
            }
        }
        num++;
        return *this;
    }

    VersiondogxsTester& TestFailed() {
        for (int i = 0; i < CHECKERS; i++) {
            if (InsecureRanddogxs(i) == 0) {
                BOOST_CHECK_MESSAGE(checker[i].GetStateFor(vpblock.empty() ? nullptr : vpblock.back()) == ThresholdState::FAILED, strprintf("Test %i for FAILED", num));
                BOOST_CHECK_MESSAGE(checker_always[i].GetStateFor(vpblock.empty() ? nullptr : vpblock.back()) == ThresholdState::ACTIVE, strprintf("Test %i for ACTIVE (always active)", num));
            }
        }
        num++;
        return *this;
    }

    CBlockIndex * Tip() { return vpblock.size() ? vpblock.back() : nullptr; }
};

BOOST_FIXTURE_TEST_SUITE(versiondogxs_tests, TestingSetup)

BOOST_AUTO_TEST_CASE(versiondogxs_test)
{
    for (int i = 0; i < 64; i++) {
        // DEFINED -> FAILED
        VersiondogxsTester().TestDefined().TestStateSinceHeight(0)
                           .Mine(1, TestTime(1), 0x100).TestDefined().TestStateSinceHeight(0)
                           .Mine(11, TestTime(11), 0x100).TestDefined().TestStateSinceHeight(0)
                           .Mine(989, TestTime(989), 0x100).TestDefined().TestStateSinceHeight(0)
                           .Mine(999, TestTime(20000), 0x100).TestDefined().TestStateSinceHeight(0)
                           .Mine(1000, TestTime(20000), 0x100).TestFailed().TestStateSinceHeight(1000)
                           .Mine(1999, TestTime(30001), 0x100).TestFailed().TestStateSinceHeight(1000)
                           .Mine(2000, TestTime(30002), 0x100).TestFailed().TestStateSinceHeight(1000)
                           .Mine(2001, TestTime(30003), 0x100).TestFailed().TestStateSinceHeight(1000)
                           .Mine(2999, TestTime(30004), 0x100).TestFailed().TestStateSinceHeight(1000)
                           .Mine(3000, TestTime(30005), 0x100).TestFailed().TestStateSinceHeight(1000)

        // DEFINED -> STARTED -> FAILED
                           .Reset().TestDefined().TestStateSinceHeight(0)
                           .Mine(1, TestTime(1), 0).TestDefined().TestStateSinceHeight(0)
                           .Mine(1000, TestTime(10000) - 1, 0x100).TestDefined().TestStateSinceHeight(0) // One second more and it would be defined
                           .Mine(2000, TestTime(10000), 0x100).TestStarted().TestStateSinceHeight(2000) // So that's what happens the next period
                           .Mine(2051, TestTime(10010), 0).TestStarted().TestStateSinceHeight(2000) // 51 old blocks
                           .Mine(2950, TestTime(10020), 0x100).TestStarted().TestStateSinceHeight(2000) // 899 new blocks
                           .Mine(3000, TestTime(20000), 0).TestFailed().TestStateSinceHeight(3000) // 50 old blocks (so 899 out of the past 1000)
                           .Mine(4000, TestTime(20010), 0x100).TestFailed().TestStateSinceHeight(3000)

        // DEFINED -> STARTED -> FAILED while threshold reached
                           .Reset().TestDefined().TestStateSinceHeight(0)
                           .Mine(1, TestTime(1), 0).TestDefined().TestStateSinceHeight(0)
                           .Mine(1000, TestTime(10000) - 1, 0x101).TestDefined().TestStateSinceHeight(0) // One second more and it would be defined
                           .Mine(2000, TestTime(10000), 0x101).TestStarted().TestStateSinceHeight(2000) // So that's what happens the next period
                           .Mine(2999, TestTime(30000), 0x100).TestStarted().TestStateSinceHeight(2000) // 999 new blocks
                           .Mine(3000, TestTime(30000), 0x100).TestFailed().TestStateSinceHeight(3000) // 1 new block (so 1000 out of the past 1000 are new)
                           .Mine(3999, TestTime(30001), 0).TestFailed().TestStateSinceHeight(3000)
                           .Mine(4000, TestTime(30002), 0).TestFailed().TestStateSinceHeight(3000)
                           .Mine(14333, TestTime(30003), 0).TestFailed().TestStateSinceHeight(3000)
                           .Mine(24000, TestTime(40000), 0).TestFailed().TestStateSinceHeight(3000)

        // DEFINED -> STARTED -> LOCKEDIN at the last minute -> ACTIVE
                           .Reset().TestDefined()
                           .Mine(1, TestTime(1), 0).TestDefined().TestStateSinceHeight(0)
                           .Mine(1000, TestTime(10000) - 1, 0x101).TestDefined().TestStateSinceHeight(0) // One second more and it would be defined
                           .Mine(2000, TestTime(10000), 0x101).TestStarted().TestStateSinceHeight(2000) // So that's what happens the next period
                           .Mine(2050, TestTime(10010), 0x200).TestStarted().TestStateSinceHeight(2000) // 50 old blocks
                           .Mine(2950, TestTime(10020), 0x100).TestStarted().TestStateSinceHeight(2000) // 900 new blocks
                           .Mine(2999, TestTime(19999), 0x200).TestStarted().TestStateSinceHeight(2000) // 49 old blocks
                           .Mine(3000, TestTime(29999), 0x200).TestLockedIn().TestStateSinceHeight(3000) // 1 old block (so 900 out of the past 1000)
                           .Mine(3999, TestTime(30001), 0).TestLockedIn().TestStateSinceHeight(3000)
                           .Mine(4000, TestTime(30002), 0).TestActive().TestStateSinceHeight(4000)
                           .Mine(14333, TestTime(30003), 0).TestActive().TestStateSinceHeight(4000)
                           .Mine(24000, TestTime(40000), 0).TestActive().TestStateSinceHeight(4000)

        // DEFINED multiple periods -> STARTED multiple periods -> FAILED
                           .Reset().TestDefined().TestStateSinceHeight(0)
                           .Mine(999, TestTime(999), 0).TestDefined().TestStateSinceHeight(0)
                           .Mine(1000, TestTime(1000), 0).TestDefined().TestStateSinceHeight(0)
                           .Mine(2000, TestTime(2000), 0).TestDefined().TestStateSinceHeight(0)
                           .Mine(3000, TestTime(10000), 0).TestStarted().TestStateSinceHeight(3000)
                           .Mine(4000, TestTime(10000), 0).TestStarted().TestStateSinceHeight(3000)
                           .Mine(5000, TestTime(10000), 0).TestStarted().TestStateSinceHeight(3000)
                           .Mine(6000, TestTime(20000), 0).TestFailed().TestStateSinceHeight(6000)
                           .Mine(7000, TestTime(20000), 0x100).TestFailed().TestStateSinceHeight(6000);
    }

    // Sanity checks of version dogx deployments
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    const Consensus::Params &mainnetParams = chainParams->GetConsensus();
    for (int i=0; i<(int) Consensus::MAX_VERSION_dogxS_DEPLOYMENTS; i++) {
        uint32_t dogxmask = VersiondogxsMask(mainnetParams, static_cast<Consensus::DeploymentPos>(i));
        // Make sure that no deployment tries to set an invalid dogx.
        BOOST_CHECK_EQUAL(dogxmask & ~(uint32_t)VERSIONdogxS_TOP_MASK, dogxmask);

        // Verify that the deployment windows of different deployment using the
        // same dogx are disjoint.
        // This test may need modification at such time as a new deployment
        // is proposed that reuses the dogx of an activated soft fork, before the
        // end time of that soft fork.  (Alternatively, the end time of that
        // activated soft fork could be later changed to be earlier to avoid
        // overlap.)
        for (int j=i+1; j<(int) Consensus::MAX_VERSION_dogxS_DEPLOYMENTS; j++) {
            if (VersiondogxsMask(mainnetParams, static_cast<Consensus::DeploymentPos>(j)) == dogxmask) {
                BOOST_CHECK(mainnetParams.vDeployments[j].nStartTime > mainnetParams.vDeployments[i].nTimeout ||
                        mainnetParams.vDeployments[i].nStartTime > mainnetParams.vDeployments[j].nTimeout);
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(versiondogxs_computeblockversion)
{
    // Check that ComputeBlockVersion will set the appropriate dogx correctly
    // on mainnet.
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    const Consensus::Params &mainnetParams = chainParams->GetConsensus();

    // Use the TESTDUMMY deployment for testing purposes.
    int64_t dogx = mainnetParams.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].dogx;
    int64_t nStartTime = mainnetParams.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime;
    int64_t nTimeout = mainnetParams.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout;

    assert(nStartTime < nTimeout);

    // In the first chain, test that the dogx is set by CBV until it has failed.
    // In the second chain, test the dogx is set by CBV while STARTED and
    // LOCKED-IN, and then no longer set while ACTIVE.
    VersiondogxsTester firstChain, secondChain;

    // Start generating blocks before nStartTime
    int64_t nTime = nStartTime - 1;

    // Before MedianTimePast of the chain has crossed nStartTime, the dogx
    // should not be set.
    CBlockIndex *lastBlock = nullptr;
    lastBlock = firstChain.Mine(2016, nTime, VERSIONdogxS_LAST_OLD_BLOCK_VERSION).Tip();
    BOOST_CHECK_EQUAL(ComputeBlockVersion(lastBlock, mainnetParams) & (1<<dogx), 0);

    // Mine 2011 more blocks at the old time, and check that CBV isn't setting the dogx yet.
    for (int i=1; i<2012; i++) {
        lastBlock = firstChain.Mine(2016+i, nTime, VERSIONdogxS_LAST_OLD_BLOCK_VERSION).Tip();
        // This works because VERSIONdogxS_LAST_OLD_BLOCK_VERSION happens
        // to be 4, and the dogx we're testing happens to be dogx 28.
        BOOST_CHECK_EQUAL(ComputeBlockVersion(lastBlock, mainnetParams) & (1<<dogx), 0);
    }
    // Now mine 5 more blocks at the start time -- MTP should not have passed yet, so
    // CBV should still not yet set the dogx.
    nTime = nStartTime;
    for (int i=2012; i<=2016; i++) {
        lastBlock = firstChain.Mine(2016+i, nTime, VERSIONdogxS_LAST_OLD_BLOCK_VERSION).Tip();
        BOOST_CHECK_EQUAL(ComputeBlockVersion(lastBlock, mainnetParams) & (1<<dogx), 0);
    }

    // Advance to the next period and transition to STARTED,
    lastBlock = firstChain.Mine(6048, nTime, VERSIONdogxS_LAST_OLD_BLOCK_VERSION).Tip();
    // so ComputeBlockVersion should now set the dogx,
    BOOST_CHECK((ComputeBlockVersion(lastBlock, mainnetParams) & (1<<dogx)) != 0);
    // and should also be using the VERSIONdogxS_TOP_dogxS.
    BOOST_CHECK_EQUAL(ComputeBlockVersion(lastBlock, mainnetParams) & VERSIONdogxS_TOP_MASK, VERSIONdogxS_TOP_dogxS);

    // Check that ComputeBlockVersion will set the dogx until nTimeout
    nTime += 600;
    int blocksToMine = 4032; // test blocks for up to 2 time periods
    int nHeight = 6048;
    // These blocks are all before nTimeout is reached.
    while (nTime < nTimeout && blocksToMine > 0) {
        lastBlock = firstChain.Mine(nHeight+1, nTime, VERSIONdogxS_LAST_OLD_BLOCK_VERSION).Tip();
        BOOST_CHECK((ComputeBlockVersion(lastBlock, mainnetParams) & (1<<dogx)) != 0);
        BOOST_CHECK_EQUAL(ComputeBlockVersion(lastBlock, mainnetParams) & VERSIONdogxS_TOP_MASK, VERSIONdogxS_TOP_dogxS);
        blocksToMine--;
        nTime += 600;
        nHeight += 1;
    }

    nTime = nTimeout;
    // FAILED is only triggered at the end of a period, so CBV should be setting
    // the dogx until the period transition.
    for (int i=0; i<2015; i++) {
        lastBlock = firstChain.Mine(nHeight+1, nTime, VERSIONdogxS_LAST_OLD_BLOCK_VERSION).Tip();
        BOOST_CHECK((ComputeBlockVersion(lastBlock, mainnetParams) & (1<<dogx)) != 0);
        nHeight += 1;
    }
    // The next block should trigger no longer setting the dogx.
    lastBlock = firstChain.Mine(nHeight+1, nTime, VERSIONdogxS_LAST_OLD_BLOCK_VERSION).Tip();
    BOOST_CHECK_EQUAL(ComputeBlockVersion(lastBlock, mainnetParams) & (1<<dogx), 0);

    // On a new chain:
    // verify that the dogx will be set after lock-in, and then stop being set
    // after activation.
    nTime = nStartTime;

    // Mine one period worth of blocks, and check that the dogx will be on for the
    // next period.
    lastBlock = secondChain.Mine(2016, nTime, VERSIONdogxS_LAST_OLD_BLOCK_VERSION).Tip();
    BOOST_CHECK((ComputeBlockVersion(lastBlock, mainnetParams) & (1<<dogx)) != 0);

    // Mine another period worth of blocks, signaling the new dogx.
    lastBlock = secondChain.Mine(4032, nTime, VERSIONdogxS_TOP_dogxS | (1<<dogx)).Tip();
    // After one period of setting the dogx on each block, it should have locked in.
    // We keep setting the dogx for one more period though, until activation.
    BOOST_CHECK((ComputeBlockVersion(lastBlock, mainnetParams) & (1<<dogx)) != 0);

    // Now check that we keep mining the block until the end of this period, and
    // then stop at the beginning of the next period.
    lastBlock = secondChain.Mine(6047, nTime, VERSIONdogxS_LAST_OLD_BLOCK_VERSION).Tip();
    BOOST_CHECK((ComputeBlockVersion(lastBlock, mainnetParams) & (1<<dogx)) != 0);
    lastBlock = secondChain.Mine(6048, nTime, VERSIONdogxS_LAST_OLD_BLOCK_VERSION).Tip();
    BOOST_CHECK_EQUAL(ComputeBlockVersion(lastBlock, mainnetParams) & (1<<dogx), 0);

    // Finally, verify that after a soft fork has activated, CBV no longer uses
    // VERSIONdogxS_LAST_OLD_BLOCK_VERSION.
    //BOOST_CHECK_EQUAL(ComputeBlockVersion(lastBlock, mainnetParams) & VERSIONdogxS_TOP_MASK, VERSIONdogxS_TOP_dogxS);
}


BOOST_AUTO_TEST_SUITE_END()
