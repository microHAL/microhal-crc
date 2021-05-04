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

#include "doctest/doctest.h"
#include "microhal-crc.hpp"

constexpr auto implementation = Implementation::CRC_IMPLEMENTATION;

// 0102030405060708090a0b0c0d0e0faabbccddeeff
constexpr std::array<uint8_t, 11> testdataPart1 = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b};
constexpr std::array<uint8_t, 10> testdataPart2 = {0x0c, 0x0d, 0x0e, 0x0f, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

TEST_CASE("CRC8 Partial computation") {
    {
        INFO("Checking CRC8_CCITT");
        auto part1Crc =
            CRC8_CCITT<implementation>::calculatePartial(CRC8_CCITT<implementation>::initialValue(true), testdataPart1.data(), testdataPart1.size());
        auto part2Crc = CRC8_CCITT<implementation>::calculatePartial(part1Crc, testdataPart2.data(), testdataPart2.size());
        auto result = CRC8_CCITT<implementation>::finalize(part2Crc);
        CHECK(result == 0x71);
    }
}

TEST_CASE("CRC16 Partial computation") {
    {
        INFO("Checking CRC16_CCITT");
        auto part1Crc = CRC16_CCITT<implementation>::calculatePartial(CRC16_CCITT<implementation>::initialValue(true), testdataPart1.data(),
                                                                      testdataPart1.size());
        auto part2Crc = CRC16_CCITT<implementation>::calculatePartial(part1Crc, testdataPart2.data(), testdataPart2.size());
        auto result = CRC16_CCITT<implementation>::finalize(part2Crc);
        CHECK(result == 0x7C8C);
    }
    {
        INFO("Checking CRC16_ARC");
        auto part1Crc =
            CRC16_ARC<implementation>::calculatePartial(CRC16_ARC<implementation>::initialValue(true), testdataPart1.data(), testdataPart1.size());
        auto part2Crc = CRC16_ARC<implementation>::calculatePartial(part1Crc, testdataPart2.data(), testdataPart2.size());
        auto result = CRC16_ARC<implementation>::finalize(part2Crc);
        CHECK(result == 0x322A);
    }
}
