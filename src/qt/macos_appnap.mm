// Copyright (c) 2011-2018 The dogxcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "macos_appnap.h"

#include <AvailabilityMacros.h>
#include <Foundation/NSProcessInfo.h>
#include <Foundation/Foundation.h>

class CAppNapInhidogxor::CAppNapImpl
{
public:
    ~CAppNapImpl()
    {
        if(activityId)
            enableAppNap();
    }

    void disableAppNap()
    {
        if (!activityId)
        {
            @autoreleasepool {
                const NSActivityOptions activityOptions =
                NSActivityUserInitiatedAllowingIdleSystemSleep &
                ~(NSActivitySuddenTerminationDisabled |
                NSActivityAutomaticTerminationDisabled);

                id processInfo = [NSProcessInfo processInfo];
                if ([processInfo respondsToSelector:@selector(beginActivityWithOptions:reason:)])
                {
                    activityId = [processInfo beginActivityWithOptions: activityOptions reason:@"Temporarily disable App Nap for dogxcoin-qt."];
                    [activityId retain];
                }
            }
        }
    }

    void enableAppNap()
    {
        if(activityId)
        {
            @autoreleasepool {
                id processInfo = [NSProcessInfo processInfo];
                if ([processInfo respondsToSelector:@selector(endActivity:)])
                    [processInfo endActivity:activityId];

                [activityId release];
                activityId = nil;
            }
        }
    }

private:
    NSObject* activityId;
};

CAppNapInhidogxor::CAppNapInhidogxor() : impl(new CAppNapImpl()) {}

CAppNapInhidogxor::~CAppNapInhidogxor() = default;

void CAppNapInhidogxor::disableAppNap()
{
    impl->disableAppNap();
}

void CAppNapInhidogxor::enableAppNap()
{
    impl->enableAppNap();
}
