// Copyright (c) 2009-2019 The dogxcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef dogxCOIN_TEST_FUZZ_FUZZ_H
#define dogxCOIN_TEST_FUZZ_FUZZ_H

#include <functional>
#include <stdint.h>
#include <vector>


void test_one_input(std::vector<uint8_t> buffer);

#endif // dogxCOIN_TEST_FUZZ_FUZZ_H
