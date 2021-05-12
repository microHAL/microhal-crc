/*
 * test_stringCrcPolynomial.cpp
 *
 *  Created on: May 7, 2021
 *      Author: pokas
 */

#include <doctest/doctest.h>
#include <microhal-crc.hpp>

using namespace microhal;

constexpr auto implementation = Implementation::CRC_IMPLEMENTATION;

TEST_CASE("Test String Polinomial decode") {
    static_assert(crcDetail::atoi("95") == 95);
    static_assert(crcDetail::atoi("5") == 5);
    static_assert(crcDetail::atoi("195") == 195);

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("x^2");
        static_assert(coef == 2);
    }

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("x ^ 2");
        static_assert(coef == 2);
    }

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("  x ^ 2   ");
        static_assert(coef == 2);
    }

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("  x    ^    2   ");
        static_assert(coef == 2);
    }

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("  x^    2   ");
        static_assert(coef == 2);
    }

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("  x    ^2   ");
        static_assert(coef == 2);
    }

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("x^0");
        static_assert(coef == 0);
    }

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("x^1");
        static_assert(coef == 1);
    }

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("x^10");
        static_assert(coef == 10);
    }

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("x^123");
        static_assert(coef == 123);
    }

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("1");
        static_assert(coef == 0);
    }

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("x^1 2 3");
        static_assert(coef == -1);
    }

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("^1");
        static_assert(coef == -1);
    }

    {
        constexpr auto coef = crcDetail::decodeCoeffitient("x^       ");
        static_assert(coef == -1);
    }

    //--------------------------------------------------------------------------
    {
        constexpr std::pair<uint64_t, size_t> a = crcDetail::stringToPoly("x^2");
        static_assert(a.first == 0b0);
        static_assert(a.second == 2);
    }

    {
        constexpr std::pair<uint64_t, size_t> a = crcDetail::stringToPoly("x^2+x^1");
        static_assert(a.first == 0b10);
        static_assert(a.second == 2);
    }

    {
        constexpr std::pair<uint64_t, size_t> a = crcDetail::stringToPoly("x^16+x^1");
        static_assert(a.first == 0b10);
        static_assert(a.second == 16);
    }

    {
        constexpr std::pair<uint64_t, size_t> a = crcDetail::stringToPoly("x^16+x^1+1");
        static_assert(a.first == 0b11);
        static_assert(a.second == 16);
    }

    {
        constexpr std::pair<uint64_t, size_t> a = crcDetail::stringToPoly("x^16+x^12+x^5+1");
        static_assert(a.first == 0b1'0000'0010'0001);
        static_assert(a.second == 16);
    }

    {
        constexpr std::pair<uint64_t, size_t> a = crcDetail::stringToPoly("x^16+x^12+x^5+x^0");
        static_assert(a.first == 0b1'0000'0010'0001);
        static_assert(a.second == 16);
    }
    {
        // 0102030405060708090a0b0c0d0e0faabbccddeeff
        constexpr std::array<uint8_t, 21> testdata = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
                                                      0x0c, 0x0d, 0x0e, 0x0f, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
        using CRC = CRC<implementation, uint16_t, "x^16+x^12+x^5+x^0">;
        constexpr auto result = CRC::calculate(testdata.data(), testdata.size());
        static_assert(CRC::polynomialLength() == 16);
        static_assert(result == 0x4B55);
    }
}
