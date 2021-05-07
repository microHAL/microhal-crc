# microhal-crc

C++20 CRC libarary tha can be used at compile time or run time.

Defining new CRC
```c++
#include "microhal-crc.hpp"

//                  <implementation>, <checksum data type>, <{polynomial, polynomial len}>
using MY_CRC = CRC<Implementation::Table256, uint16_t, {0x8BB7, 16}>; // equal to CRC16_T10_DIF

//                  <implementation>, <checksum data type>, <{polynomial, polynomial len}, <initial value>, <output xor value>, <properties>
using MY_CRC2 = CRC<Implementation::Table256, uint32_t, {0xA833982B, 32}, 0xFFFFFFFF, 0xFFFFFFFF, Properties::ReflectIn | Properties::ReflectOut>; // equal to CRC32D

using MY_CRC3 = CRC<Implementation::Table256, uint16_t, "x^16+x^12+x^5+x^0">; 
```

Usage example, calculate CRC8_CCITT with default implementation
```c++
#include "microhal-crc.hpp"

void calcCrc(){
std::array<uint8_t, 4> data = {0,1,2,3};

auto crc = CRC8_CCITT<>::calculate(data.data(), data.size());
}
```

you can specified CRC implementation to one from: BitShift, BitShiftLsb, Table256, Table256Lsb example bellow show CRC8_CCITT calculation with Table256 implementation
```c++
#include "microhal-crc.hpp"

void calcCrc(){
std::array<uint8_t, 4> data = {0,1,2,3};

auto crc = CRC8_CCITT<Implementation::Table256>::calculate(data.data(), data.size());
}
```
