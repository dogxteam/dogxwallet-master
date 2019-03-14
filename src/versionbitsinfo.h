// Copyright (c) 2016-2018 The dogxcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef dogxCOIN_VERSIONdogxSINFO_H
#define dogxCOIN_VERSIONdogxSINFO_H

struct VBDeploymentInfo {
    /** Deployment name */
    const char *name;
    /** Whether GBT clients can safely ignore this rule in simplified usage */
    bool gbt_force;
};

extern const struct VBDeploymentInfo VersiondogxsDeploymentInfo[];

#endif // dogxCOIN_VERSIONdogxSINFO_H
