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

using namespace microhal;

constexpr auto implementation = Implementation::CRC_IMPLEMENTATION;

TEST_CASE("CRC7 'x^7 + X^3 + 1' used in SD Card") {
    {
        uint8_t data[5] = {0b0100'0000, 0x00, 0x00, 0x00, 0x00};
        CHECK(CRC7<implementation>::calculate(data, sizeof(data)) == 0b100'1010);
    }
    {
        uint8_t data[5] = {0b0101'0001, 0x00, 0x00, 0x00, 0x00};
        CHECK(CRC7<implementation>::calculate(data, sizeof(data)) == 0b010'1010);
    }
    {
        uint8_t data[5] = {0b0001'0001, 0x00, 0x00, 0x09, 0x00};
        CHECK(CRC7<implementation>::calculate(data, sizeof(data)) == 0b011'0011);
    }
}

TEST_CASE("CRC16 'x^16 + x^12 + x^5 + 1' used in SD Card") {
    {
        std::array<uint8_t, 512> data;
        data.fill(0xFF);
        uint16_t crc_calculated = CRC16_XMODEM<implementation>::calculate(data.data(), data.size());
        CHECK(crc_calculated == 0x7FA1);
    }
}
