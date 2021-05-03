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
constexpr std::array<uint8_t, 21> testdata = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
                                              0x0c, 0x0d, 0x0e, 0x0f, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

TEST_CASE("CRC8") {
    {
        INFO("Checking CRC8_CCITT");
        auto result = CRC8_CCITT<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x71);
    }
    {
        INFO("Checking CRC8_CDMA2000, init value 0xFF");
        auto result = CRC8_CDMA2000<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xF9);
    }
    {
        INFO("Checking CRC8_DARC");
        auto result = CRC8_DARC<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x07);
    }
    {
        INFO("Checking CRC8_DVB_S2");
        auto result = CRC8_DVB_S2<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x11);
    }
    {
        INFO("Checking CRC8_EBU, init value 0xFF");
        auto result = CRC8_EBU<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x4C);
    }
    {
        INFO("Checking CRC8_I_CODE, init value 0xFD");
        auto result = CRC8_I_CODE<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x3F);
    }
    {
        INFO("Checking CRC8_ITU");
        auto result = CRC8_ITU<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x24);
    }
    {
        INFO("Checking CRC8_MAXIM");
        auto result = CRC8_MAXIM<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x82);
    }
    {
        INFO("Checking CRC8_ROHC, init value 0xFF");
        auto result = CRC8_ROHC<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xB1);
    }
    {
        INFO("Checking CRC8_WCDMA");
        auto result = CRC8_WCDMA<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xB2);
    }
}

TEST_CASE("CRC16") {
    {
        INFO("Checking CRC16_CCITT");
        auto result = CRC16_CCITT<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x7C8C);
    }
    {
        INFO("Checking CRC16_ARC");
        auto result = CRC16_ARC<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x322A);
    }
    {
        INFO("Checking CRC16_AUG_CCITT, init 0x1D0F");
        auto result = CRC16_AUG_CCITT<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x8D03);
    }
    {
        INFO("Checking CRC16_BUYPASS");
        auto result = CRC16_BUYPASS<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x2913);
    }
    {
        INFO("Checking CRC16_CDMA2000");
        auto result = CRC16_CDMA2000<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x6204);
    }
    {
        INFO("Checking CRC16_DDS_110");
        auto result = CRC16_DDS_110<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x7BCC);
    }
    {
        INFO("Checking CRC16_DECT_R");
        auto result = CRC16_DECT_R<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xFD81);
    }
    {
        INFO("Checking CRC16_DECT_X");
        auto result = CRC16_DECT_X<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xFD80);
    }
    {
        INFO("Checking CRC16_DNP");
        auto result = CRC16_DNP<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xD288);
    }
    {
        INFO("Checking CRC16_EN_13757");
        auto result = CRC16_EN_13757<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x07E6);
    }
    {
        INFO("Checking CRC16_GENIBUS");
        auto result = CRC16_GENIBUS<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x8373);
    }
    {
        INFO("Checking CRC16_MAXIM");
        auto result = CRC16_MAXIM<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xCDD5);
    }
    {
        INFO("Checking CRC16_MCRF4XX");
        auto result = CRC16_MCRF4XX<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xA041);
    }
    {
        INFO("Checking CRC16_RIELLO");
        auto result = CRC16_RIELLO<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x908B);
    }
    {
        INFO("Checking CRC16_T10_DIF");
        auto result = CRC16_T10_DIF<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xE564);
    }
    {
        INFO("Checking CRC16_TELEDISK");
        auto result = CRC16_TELEDISK<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x024C);
    }
    {
        INFO("Checking CRC16_TMS37157");
        auto result = CRC16_TMS37157<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xAFCF);
    }
    {
        INFO("Checking CRC16_USB");
        auto result = CRC16_USB<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xD6CE);
    }
    {
        INFO("Checking CRC16_A");
        auto result = CRC16_A<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x3074);
    }
    {
        INFO("Checking CRC16_KERMIT");
        auto result = CRC16_KERMIT<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x3BAD);
    }
    {
        INFO("Checking CRC16_MODBUS");
        auto result = CRC16_MODBUS<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x2931);
    }
    {
        INFO("Checking CRC16_X_25");
        auto result = CRC16_X_25<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x5FBE);
    }
    {
        INFO("Checking CRC16_XMODEM");
        auto result = CRC16_XMODEM<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x4B55);
    }
}

TEST_CASE("CRC32") {
    {
        INFO("Checking CRC32_BZIP2");
        auto result = CRC32_BZIP2<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xF7AC52F1);
    }
    {
        INFO("Checking CRC32C");
        auto result = CRC32C<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xE26FCA95);
    }
    {
        INFO("Checking CRC32D");
        auto result = CRC32D<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x58CB70A9);
    }
    {
        INFO("Checking CRC32_MPEG_2");
        auto result = CRC32_MPEG_2<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x0853AD0E);
    }
    {
        INFO("Checking CRC32_POSIX");
        auto result = CRC32_POSIX<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xD9F08D8C);
    }
    {
        INFO("Checking CRC32Q");
        auto result = CRC32Q<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x0ED9B182);
    }
    {
        INFO("Checking CRC32_JAMCRC");
        auto result = CRC32_JAMCRC<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0x81E1C7FD);
    }
    {
        INFO("Checking CRC32_XFER");
        auto result = CRC32_XFER<implementation>::calculate(testdata.data(), testdata.size());
        CHECK(result == 0xC54B7959);
    }
}
