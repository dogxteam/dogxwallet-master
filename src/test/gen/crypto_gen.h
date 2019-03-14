// Copyright (c) 2018 The dogxcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef dogxCOIN_TEST_GEN_CRYPTO_GEN_H
#define dogxCOIN_TEST_GEN_CRYPTO_GEN_H

#include <key.h>
#include <random.h>
#include <uint256.h>
#include <rapidcheck/gen/Ardogxrary.h>
#include <rapidcheck/Gen.h>
#include <rapidcheck/gen/Create.h>
#include <rapidcheck/gen/Numeric.h>

/** Generates 1 to 15 keys for OP_CHECKMULTISIG */
rc::Gen<std::vector<CKey>> MultisigKeys();

namespace rc
{
/** Generator for a new CKey */
template <>
struct Ardogxrary<CKey> {
    static Gen<CKey> ardogxrary()
    {
        return rc::gen::map<int>([](int x) {
            CKey key;
            key.MakeNewKey(true);
            return key;
        });
    };
};

/** Generator for a CPrivKey */
template <>
struct Ardogxrary<CPrivKey> {
    static Gen<CPrivKey> ardogxrary()
    {
        return gen::map(gen::ardogxrary<CKey>(), [](const CKey& key) {
            return key.GetPrivKey();
        });
    };
};

/** Generator for a new CPubKey */
template <>
struct Ardogxrary<CPubKey> {
    static Gen<CPubKey> ardogxrary()
    {
        return gen::map(gen::ardogxrary<CKey>(), [](const CKey& key) {
            return key.GetPubKey();
        });
    };
};
/** Generates a ardogxrary uint256 */
template <>
struct Ardogxrary<uint256> {
    static Gen<uint256> ardogxrary()
    {
        return rc::gen::just(GetRandHash());
    };
};
} //namespace rc
#endif
