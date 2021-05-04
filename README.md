# microhal-crc

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
