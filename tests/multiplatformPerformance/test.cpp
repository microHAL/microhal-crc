/**
 * @license    BSD 3-Clause
 * @brief      CRC library
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2021, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the
 * following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <array>
#include <chrono>
#include <iostream>
#include <microhal-crc.hpp>
#include <nlohmann/json.hpp>

#ifdef EMBEDDED_TARGET
#include "bsp/nucleo-f411re/bsp.h"
#include "microhal.h"
#undef CRC
#endif

using namespace microhal;

using json = nlohmann::json;

static std::array<uint8_t, 100'000> message;

template <class CRC>
std::chrono::nanoseconds crcRuntimeTest() {
    auto start = std::chrono::steady_clock::now();
    for (size_t i = 10; i > 0; --i) {
        [[maybe_unused]] volatile auto result = CRC::calculate(message.data(), message.size());
    }
    auto stop = std::chrono::steady_clock::now();

    return stop - start;
}

template <template <Implementation> class CRC>
json crcTest() {
    json test;
    test["CRC"]["poly"] = CRC<Implementation::BitShift>::polynomial();
    test["CRC"]["polyLength"] = CRC<Implementation::BitShift>::polynomialLength();
    test["CRC"]["inputReflected"] = CRC<Implementation::BitShift>::inputReflected();
    test["CRC"]["outputReflected"] = CRC<Implementation::BitShift>::outputReflected();
    test["CRC"]["initialValue"] = CRC<Implementation::BitShift>::initialValue(false);

    test["implementation"]["BitShift"] = crcRuntimeTest<CRC<Implementation::BitShift>>().count();
    test["implementation"]["BitShiftLsb"] = crcRuntimeTest<CRC<Implementation::BitShiftLsb>>().count();
    test["implementation"]["Table256"] = crcRuntimeTest<CRC<Implementation::Table256>>().count();
    test["implementation"]["Table256Lsb"] = crcRuntimeTest<CRC<Implementation::Table256Lsb>>().count();
    return test;
}

int main() {
    message.fill(0xAA);

    json testResult;
    std::array<json, 6> tests;

    testResult["Compiler"] = "GCC";

    tests[0] = crcTest<CRC8_CCITT>();
    tests[0]["CRC"]["name"] = "CRC8_CCITT";

    tests[1] = crcTest<CRC8_DARC>();
    tests[1]["CRC"]["name"] = "CRC8_DARC";

    tests[2] = crcTest<CRC16_CCITT>();
    tests[2]["CRC"]["name"] = "CRC16_CCITT";

    tests[3] = crcTest<CRC16_ARC>();
    tests[3]["CRC"]["name"] = "CRC16_ARC";

    tests[4] = crcTest<CRC32>();
    tests[4]["CRC"]["name"] = "CRC32";

    tests[5] = crcTest<CRC32C>();
    tests[5]["CRC"]["name"] = "CRC32C";

    testResult = tests;

#ifdef EMBEDDED_TARGET
    bsp::debugSerial.write(testResult.dump());
#else
    std::cout << testResult;
#endif

    return 0;
}
