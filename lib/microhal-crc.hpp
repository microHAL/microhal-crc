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

#ifndef _MICROHAL_CRC_H_
#define _MICROHAL_CRC_H_

#include <array>
#include <cstdint>

#undef CRC

namespace microhal {

namespace detail {
template <typename ChecksumType>
static constexpr ChecksumType maskGen(size_t len) {
    ChecksumType mask = 1;
    for (size_t i = 0; i < len - 1; i++) {
        mask <<= 1;
        mask += 1;
    }
    return mask;
}

constexpr uint16_t reverseBits(uint16_t x) {
    x = (((x & 0xaaaa) >> 1) | ((x & 0x5555) << 1));
    x = (((x & 0xcccc) >> 2) | ((x & 0x3333) << 2));
    x = (((x & 0xf0f0) >> 4) | ((x & 0x0f0f) << 4));
    x = (((x & 0xff00) >> 8) | ((x & 0x00ff) << 8));

    return x;
}

constexpr uint8_t reverseBits(uint8_t x) {
    x = (((x & 0xaa) >> 1) | ((x & 0x55) << 1));
    x = (((x & 0xcc) >> 2) | ((x & 0x33) << 2));
    x = (x >> 4) | (x << 4);

    return x;
}

constexpr uint32_t reverseBits(uint32_t x) {
    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
    x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));

    return ((x >> 16) | (x << 16));
}

template <typename T>
constexpr std::array<T, 256> tableGeneratorMSB(T polynomial, size_t polynomialLen) {
    // This function is always run at compile time so we don't need to do
    // run time optimization
    const size_t shiftToAlign8Bit = ((sizeof(T) * 8 - polynomialLen) % 8);
    std::array<T, 256> msbTable{};
    const T polinomialMsbBitSet = 1U << (polynomialLen - 1 + shiftToAlign8Bit);
    size_t i = 1;
    do {
        T crc = i;

        for (size_t bit = polynomialLen + shiftToAlign8Bit; bit > 0; --bit) {
            if (crc & polinomialMsbBitSet) {
                crc = (crc << 1) ^ (polynomial << shiftToAlign8Bit);
            } else {
                crc = crc << 1;
            }
        }
        msbTable[i] = crc;
        i++;
    } while (i < 256);

    return msbTable;
}

template <typename T>
constexpr std::array<T, 256> tableGeneratorLSB(T polynomial, size_t polynomialLen) {
    // This function is always run at compile time so we don't need to do
    // run time optimization
    std::array<T, 256> lsbTable{};
    T mask = maskGen<T>(polynomialLen);
    for (size_t divident = 0; divident < 256; divident++) {
        T crc = divident;
        for (uint_fast8_t bit = 0; bit < 8; bit++) {
            if (crc & 0b1) {
                crc = (crc >> 1) ^ polynomial;
            } else {
                crc = crc >> 1;
            }
        }

        lsbTable[divident] = crc & mask;
    }

    return lsbTable;
}

static_assert(reverseBits(uint8_t(0x0F)) == 0xF0);
static_assert(reverseBits(uint8_t(0x01)) == 0x80);
static_assert(reverseBits(uint8_t(0x80)) == 0x01);
static_assert(reverseBits(uint8_t(0x40)) == 0x02);
static_assert(reverseBits(uint8_t(0x20)) == 0x04);
static_assert(reverseBits(uint8_t(0x10)) == 0x08);

}  // namespace detail

enum class Implementation { BitShift, BitShiftLsb, Table256, Table256Lsb };
enum class Properties { ReflectIn = 0b01, ReflectOut = 0b10, None = 0 };

constexpr Properties operator|(Properties a, Properties b) {
    return static_cast<Properties>(static_cast<uint_fast8_t>(a) | static_cast<uint_fast8_t>(b));
}

constexpr Properties operator&(Properties a, Properties b) {
    return static_cast<Properties>(static_cast<uint_fast8_t>(a) & static_cast<uint_fast8_t>(b));
}

template <Implementation impl, typename ChecksumType, ChecksumType polynomial, size_t len, bool reflectIn>
class CRCImpl;

//------------------------------------------------------------------------------
//            Bit shift implementation (slow but low footprint)
//------------------------------------------------------------------------------
template <typename ChecksumType, ChecksumType polynomial, size_t len, bool reflectIn>
class CRCImpl<Implementation::BitShift, ChecksumType, polynomial, len, reflectIn> {
 public:
    static_assert(sizeof(ChecksumType) * 8 >= len);

    static constexpr ChecksumType calculatePartial(ChecksumType init, const uint8_t *data, size_t lne) {
        ChecksumType remainder = init;
        if constexpr (reflectIn) {
            for (size_t byte = 0; byte < lne; byte++) {
                if constexpr (len <= 8) {
                    remainder = calculateByte(detail::reverseBits(data[byte]) ^ remainder);
                } else {
                    ChecksumType reversedData = ChecksumType(detail::reverseBits(data[byte])) << (len - 8);
                    remainder = calculateByte(reversedData ^ remainder);
                }
            }
        } else {
            for (size_t byte = 0; byte < lne; byte++) {
                if constexpr (len <= 8) {
                    remainder = calculateByte(data[byte] ^ remainder);
                } else {
                    ChecksumType dataTmp = ChecksumType(data[byte]) << (len - 8);
                    remainder = calculateByte(dataTmp ^ remainder);
                }
            }
        }
        return remainder >> ShiftToAlign8Bit;
    }

 private:
    static constexpr ChecksumType calculateByte(ChecksumType data) {
        ChecksumType remainder = data;
        for (size_t i = 0; i < 8; i++) {
            if (remainder & MSBBitSet) {
                remainder = (remainder << 1) ^ Polynomial;
            } else
                remainder <<= 1;
        }
        return remainder;
    }

    enum : ChecksumType {
        Mask = detail::maskGen<ChecksumType>(len),
        ShiftToAlign8Bit = ((sizeof(ChecksumType) * 8 - len) % 8),
        Polynomial = polynomial << ShiftToAlign8Bit,
        MSBBitSet = 1U << (len - 1 + ShiftToAlign8Bit)
    };
};

template <typename ChecksumType, ChecksumType polynomial, size_t len, bool reflectIn>
class CRCImpl<Implementation::BitShiftLsb, ChecksumType, polynomial, len, reflectIn> {
 public:
    static_assert(sizeof(ChecksumType) * 8 >= len);

    static constexpr ChecksumType calculatePartial(ChecksumType init, const uint8_t *data, size_t lne) {
        ChecksumType remainder = init;
        if constexpr (reflectIn) {
            for (size_t byte = 0; byte < lne; byte++) {
                remainder = calculateByte(data[byte] ^ remainder);
            }
        } else {
            for (size_t byte = 0; byte < lne; byte++) {
                remainder = calculateByte(detail::reverseBits(data[byte]) ^ remainder);
            }
        }
        return remainder;
    }

 private:
    static constexpr ChecksumType calculateByte(ChecksumType data) {
        ChecksumType remainder = data;
        for (size_t i = 0; i < 8; i++) {
            if (remainder & 0b1) {
                remainder = (remainder >> 1) ^ Polynomial;
            } else
                remainder >>= 1;
        }
        return remainder;
    }

    enum : ChecksumType {
        Mask = detail::maskGen<ChecksumType>(len),
        ShiftToAlign8Bit = ((sizeof(ChecksumType) * 8 - len) % 8),
        Polynomial = detail::reverseBits(ChecksumType(polynomial << ShiftToAlign8Bit)),
    };
};

//------------------------------------------------------------------------------
//      Table with 256 elements implementation (fast but high footprint)
//------------------------------------------------------------------------------
template <typename ChecksumType, ChecksumType polynomial, size_t len, bool reflectIn>
class CRCImpl<Implementation::Table256, ChecksumType, polynomial, len, reflectIn> {
    static_assert(sizeof(ChecksumType) * 8 >= len);
    constexpr static auto crc_table = detail::tableGeneratorMSB(polynomial, len);

 public:
    static constexpr ChecksumType calculatePartial(ChecksumType init, const uint8_t *data, size_t lne) {
        auto tableIndex = [](ChecksumType remainder, uint8_t newData) {
            if constexpr (reflectIn) {
                newData = detail::reverseBits(newData);
            }
            if constexpr (len <= 8) {
                const uint_fast8_t index = newData ^ remainder;
                return index;
            } else {
                const uint_fast8_t index = (newData ^ (remainder >> (len - 8))) & 0xFF;
                return index;
            }
        };

        ChecksumType result = init;
        for (size_t byte = 0; byte < lne; byte++) {
            result = (result << 8) ^ crc_table[tableIndex(result, data[byte])];
        }

        return result >> ShiftToAlign8Bit;
    }

 private:
    enum : ChecksumType {
        Mask = detail::maskGen<ChecksumType>(len),
        ShiftToAlign8Bit = ((sizeof(ChecksumType) * 8 - len) % 8),
    };
};
//------------------------------------------------------------------------------
//      Table with 256 elements implementation (fast but high footprint)
//------------------------------------------------------------------------------
template <typename ChecksumType, ChecksumType polynomial, size_t len, bool reflectIn>
class CRCImpl<Implementation::Table256Lsb, ChecksumType, polynomial, len, reflectIn> {
    static_assert(sizeof(ChecksumType) * 8 >= len);
    constexpr static auto crc_table = detail::tableGeneratorLSB(
        detail::reverseBits(ChecksumType(polynomial << ((sizeof(ChecksumType) * 8 - len) % 8))), len);

 public:
    static constexpr ChecksumType calculatePartial(ChecksumType init, const uint8_t *data, size_t lne) {
        auto tableIndex = [](ChecksumType remainder, uint8_t newData) {
            if constexpr (!reflectIn) {
                newData = detail::reverseBits(newData);
            }
            const uint_fast8_t index = newData ^ (remainder & 0xFFU);
            return index;
        };

        ChecksumType result = init;
        for (size_t byte = 0; byte < lne; byte++) {
            result = (result >> 8) ^ crc_table[tableIndex(result, data[byte])];
        }

        return result;
    }

 private:
    enum : ChecksumType {
        Mask = detail::maskGen<ChecksumType>(len),
        ShiftToAlign8Bit = ((sizeof(ChecksumType) * 8 - len) % 8),
    };
};

template <Implementation implementation, typename ChecksumType, ChecksumType poly, size_t len, ChecksumType initial = 0,
          ChecksumType xorOut = 0, Properties properties = Properties::None>
class CRC : public CRCImpl<implementation, ChecksumType, poly, len,
                           (properties & Properties::ReflectIn) == Properties::ReflectIn> {
    static constexpr bool isMsbImplementation() {
        return implementation == Implementation::BitShift || implementation == Implementation::Table256;
    }

 public:
    static constexpr ChecksumType polynomial() { return poly; }
    static constexpr size_t polynomialLength() { return len; }
    static constexpr ChecksumType initialValue([[maybe_unused]] bool reverseIfRequired) {
        if constexpr (isMsbImplementation()) {
            return initial;
        } else {
            if (reverseIfRequired) {
                return detail::reverseBits(initial);
            } else {
                return initial;
            }
        }
    }
    static constexpr bool inputReflected() { return (properties & Properties::ReflectIn) == Properties::ReflectIn; }
    static constexpr bool outputReflected() { return (properties & Properties::ReflectOut) == Properties::ReflectOut; }

    static constexpr ChecksumType finalize(ChecksumType remainder) {
        if constexpr (outputReflected() == isMsbImplementation()) {
            remainder = detail::reverseBits(remainder) >> ShiftToAlign8Bit;
        }

        return remainder ^ xorOut;
    }

    static constexpr ChecksumType calculate(const uint8_t *data, size_t lne) {
        ChecksumType remainder = CRC::calculatePartial(initialValue(true), data, lne);
        return finalize(remainder);
    }

 private:
    enum : ChecksumType {
        Mask = detail::maskGen<ChecksumType>(len),
        ShiftToAlign8Bit = ((sizeof(ChecksumType) * 8 - len) % 8),
    };
};

//---------------------------------------------------------------------------------------------------------------------
//                                                   Predefined CRC functions
//---------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//                                 CRC-3
//------------------------------------------------------------------------------
template <Implementation impl = Implementation::Table256>
using CRC3_GSM = CRC<impl, uint8_t, 0x3, 3>;
//------------------------------------------------------------------------------
//                                 CRC-4
//------------------------------------------------------------------------------
template <Implementation impl = Implementation::Table256>
using CRC4_ITU = CRC<impl, uint8_t, 0x3, 4>;
//------------------------------------------------------------------------------
//                                 CRC-5
//------------------------------------------------------------------------------
template <Implementation impl = Implementation::Table256>
using CRC5_EPC = CRC<impl, uint8_t, 0x09, 5>;
template <Implementation impl = Implementation::Table256>
using CRC5_ITU = CRC<impl, uint8_t, 0x15, 5>;
template <Implementation impl = Implementation::Table256>
using CRC5_USB = CRC<impl, uint8_t, 0x05, 4>;
//------------------------------------------------------------------------------
//                                 CRC-7
//------------------------------------------------------------------------------
template <Implementation impl = Implementation::Table256>
using CRC7 = CRC<impl, uint8_t, 0x09, 7>;

template <Implementation impl = Implementation::Table256>
using CRC7_MVB = CRC<impl, uint8_t, 0x65, 7>;
//------------------------------------------------------------------------------
//                                 CRC-8
//------------------------------------------------------------------------------
template <Implementation impl = Implementation::Table256>
using CRC8_CCITT = CRC<impl, uint8_t, 0x07, 8>;

template <Implementation impl = Implementation::Table256>
using CRC8_CDMA2000 = CRC<impl, uint8_t, 0x9B, 8, 0xFF>;

template <Implementation impl = Implementation::Table256>
using CRC8_DARC = CRC<impl, uint8_t, 0x39, 8, 0x00, 0x00, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC8_DVB_S2 = CRC<impl, uint8_t, 0xD5, 8>;

template <Implementation impl = Implementation::Table256>
using CRC8_EBU = CRC<impl, uint8_t, 0x1D, 8, 0xFF, 0x00, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC8_I_CODE = CRC<impl, uint8_t, 0x1D, 8, 0xFD>;

template <Implementation impl = Implementation::Table256>
using CRC8_ITU = CRC<impl, uint8_t, 0x07, 8, 0x00, 0x55>;

template <Implementation impl = Implementation::Table256>
using CRC8_MAXIM = CRC<impl, uint8_t, 0x31, 8, 0x00, 0x00, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC8_ROHC = CRC<impl, uint8_t, 0x07, 8, 0xff, 0x00, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC8_WCDMA = CRC<impl, uint8_t, 0x9B, 8, 0x00, 0x00, Properties::ReflectIn | Properties::ReflectOut>;

//------------------------------------------------------------------------------
//                                 CRC-16
//------------------------------------------------------------------------------
template <Implementation impl = Implementation::Table256>
using CRC16_CCITT = CRC<impl, uint16_t, 0x1021, 16, 0xFFFF, 0x0000>;

template <Implementation impl = Implementation::Table256>
using CRC16_ARC = CRC<impl, uint16_t, 0x8005, 16, 0x0000, 0x0000, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC16_AUG_CCITT = CRC<impl, uint16_t, 0x1021, 16, 0x1D0F>;

template <Implementation impl = Implementation::Table256>
using CRC16_BUYPASS = CRC<impl, uint16_t, 0x8005, 16>;

template <Implementation impl = Implementation::Table256>
using CRC16_DECT = CRC<impl, uint16_t, 0x0589, 16>;

template <Implementation impl = Implementation::Table256>
using CRC16_CDMA2000 = CRC<impl, uint16_t, 0xC867, 16, 0xFFFF>;

template <Implementation impl = Implementation::Table256>
using CRC16_DDS_110 = CRC<impl, uint16_t, 0x8005, 16, 0x800D>;

template <Implementation impl = Implementation::Table256>
using CRC16_DECT_R = CRC<impl, uint16_t, 0x0589, 16, 0x0000, 0x0001>;

template <Implementation impl = Implementation::Table256>
using CRC16_DECT_X = CRC<impl, uint16_t, 0x0589, 16>;

template <Implementation impl = Implementation::Table256>
using CRC16_DNP = CRC<impl, uint16_t, 0x3D65, 16, 0x0000, 0xFFFF, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC16_EN_13757 = CRC<impl, uint16_t, 0x3D65, 16, 0x0000, 0xFFFF>;

template <Implementation impl = Implementation::Table256>
using CRC16_GENIBUS = CRC<impl, uint16_t, 0x1021, 16, 0xFFFF, 0xFFFF>;

template <Implementation impl = Implementation::Table256>
using CRC16_MAXIM = CRC<impl, uint16_t, 0x8005, 16, 0x0000, 0xFFFF, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC16_MCRF4XX = CRC<impl, uint16_t, 0x1021, 16, 0xFFFF, 0x0000, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC16_RIELLO = CRC<impl, uint16_t, 0x1021, 16, 0xB2AA, 0x0000, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC16_T10_DIF = CRC<impl, uint16_t, 0x8BB7, 16>;

template <Implementation impl = Implementation::Table256>
using CRC16_TELEDISK = CRC<impl, uint16_t, 0xA097, 16>;

template <Implementation impl = Implementation::Table256>
using CRC16_TMS37157 = CRC<impl, uint16_t, 0x1021, 16, 0x89EC, 0x0000, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC16_USB = CRC<impl, uint16_t, 0x8005, 16, 0xFFFF, 0xFFFF, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC16_A = CRC<impl, uint16_t, 0x1021, 16, 0xC6C6, 0x0000, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC16_KERMIT = CRC<impl, uint16_t, 0x1021, 16, 0x0000, 0x0000, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC16_MODBUS = CRC<impl, uint16_t, 0x8005, 16, 0xFFFF, 0x0000, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC16_X_25 = CRC<impl, uint16_t, 0x1021, 16, 0xFFFF, 0xFFFF, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC16_XMODEM = CRC<impl, uint16_t, 0x1021, 16>;
//------------------------------------------------------------------------------
//                                 CRC-17
//------------------------------------------------------------------------------
template <Implementation impl = Implementation::Table256>
using CRC17_CAN = CRC<impl, uint32_t, 0x1685B, 17>;

template <Implementation impl = Implementation::Table256>
using CRC21_CAN = CRC<impl, uint32_t, 0x102899, 21>;
//------------------------------------------------------------------------------
//                                 CRC-32
//------------------------------------------------------------------------------
template <Implementation impl = Implementation::Table256>
using CRC32 = CRC<impl, uint32_t, 0x04C11DB7, 32>;

template <Implementation impl = Implementation::Table256>
using CRC32_BZIP2 = CRC<impl, uint32_t, 0x04C11DB7, 32, 0xFFFFFFFF, 0xFFFFFFFF>;

template <Implementation impl = Implementation::Table256>
using CRC32C =
    CRC<impl, uint32_t, 0x1EDC6F41, 32, 0xFFFFFFFF, 0xFFFFFFFF, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC32D =
    CRC<impl, uint32_t, 0xA833982B, 32, 0xFFFFFFFF, 0xFFFFFFFF, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC32_MPEG_2 = CRC<impl, uint32_t, 0x04C11DB7, 32, 0xFFFFFFFF, 0x00000000>;

template <Implementation impl = Implementation::Table256>
using CRC32_POSIX = CRC<impl, uint32_t, 0x04C11DB7, 32, 0x00000000, 0xFFFFFFFF>;

template <Implementation impl = Implementation::Table256>
using CRC32Q = CRC<impl, uint32_t, 0x814141AB, 32, 0x00000000, 0x00000000>;

template <Implementation impl = Implementation::Table256>
using CRC32_JAMCRC =
    CRC<impl, uint32_t, 0x04C11DB7, 32, 0xFFFFFFFF, 0x00000000, Properties::ReflectIn | Properties::ReflectOut>;

template <Implementation impl = Implementation::Table256>
using CRC32_XFER = CRC<impl, uint32_t, 0x000000AF, 32, 0x00000000, 0x00000000>;

}  // namespace microhal

#endif /* _MICROHAL_CRC_H_ */
