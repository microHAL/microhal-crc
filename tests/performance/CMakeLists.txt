set(CRC_TABLE_SRCS    
    crc_table256.cpp
    )
  
set(CRC_TABLELSB_SRCS    
    crc_table256Lsb.cpp
    )

set(CRC_BITSHIFT_SRCS    
    crc_bitShift.cpp
    )
    
set(CRC_BITSHIFTLSB_SRCS    
    crc_bitShiftLsb.cpp
    )


add_executable(perfTest_CRC7_table ${CRC_TABLE_SRCS})
target_link_libraries(perfTest_CRC7_table microhal-crc)
target_compile_definitions(perfTest_CRC7_table PUBLIC CRC_TYPE=CRC7)

add_executable(perfTest_CRC7_tableLsb ${CRC_TABLELSB_SRCS})
target_link_libraries(perfTest_CRC7_tableLsb microhal-crc)
target_compile_definitions(perfTest_CRC7_tableLsb PUBLIC CRC_TYPE=CRC7)
   
add_executable(perfTest_CRC7_bitShift ${CRC_BITSHIFT_SRCS})
target_link_libraries(perfTest_CRC7_bitShift microhal-crc)
target_compile_definitions(perfTest_CRC7_bitShift PUBLIC CRC_TYPE=CRC7)

add_executable(perfTest_CRC7_bitShiftLsb ${CRC_BITSHIFTLSB_SRCS})
target_link_libraries(perfTest_CRC7_bitShiftLsb microhal-crc)
target_compile_definitions(perfTest_CRC7_bitShiftLsb PUBLIC CRC_TYPE=CRC7)

add_executable(perfTest_CRC8_table ${CRC_TABLE_SRCS})
target_link_libraries(perfTest_CRC8_table microhal-crc)
target_compile_definitions(perfTest_CRC8_table PUBLIC CRC_TYPE=CRC8_CCITT)
   
add_executable(perfTest_CRC8_bitShift ${CRC_BITSHIFT_SRCS})
target_link_libraries(perfTest_CRC8_bitShift microhal-crc)
target_compile_definitions(perfTest_CRC8_bitShift PUBLIC CRC_TYPE=CRC8_CCITT)

add_executable(perfTest_CRC8_bitShiftLsb ${CRC_BITSHIFTLSB_SRCS})
target_link_libraries(perfTest_CRC8_bitShiftLsb microhal-crc)
target_compile_definitions(perfTest_CRC8_bitShiftLsb PUBLIC CRC_TYPE=CRC8_CCITT)

add_executable(perfTest_CRC16_table ${CRC_TABLE_SRCS})
target_link_libraries(perfTest_CRC16_table microhal-crc)
target_compile_definitions(perfTest_CRC16_table PUBLIC CRC_TYPE=CRC16_CCITT)
   
add_executable(perfTest_CRC16_bitShift ${CRC_BITSHIFT_SRCS})
target_link_libraries(perfTest_CRC16_bitShift microhal-crc)
target_compile_definitions(perfTest_CRC16_bitShift PUBLIC CRC_TYPE=CRC16_CCITT)

add_executable(perfTest_CRC16_bitShiftLsb ${CRC_BITSHIFTLSB_SRCS})
target_link_libraries(perfTest_CRC16_bitShiftLsb microhal-crc)
target_compile_definitions(perfTest_CRC16_bitShiftLsb PUBLIC CRC_TYPE=CRC16_CCITT)

add_executable(perfTest_CRC32_table ${CRC_TABLE_SRCS})
target_link_libraries(perfTest_CRC32_table microhal-crc)
target_compile_definitions(perfTest_CRC32_table PUBLIC CRC_TYPE=CRC32)
   
add_executable(perfTest_CRC32_bitShift ${CRC_BITSHIFT_SRCS})
target_link_libraries(perfTest_CRC32_bitShift microhal-crc)
target_compile_definitions(perfTest_CRC32_bitShift PUBLIC CRC_TYPE=CRC32) 

add_executable(perfTest_CRC32_bitShiftLsb ${CRC_BITSHIFTLSB_SRCS})
target_link_libraries(perfTest_CRC32_bitShiftLsb microhal-crc)
target_compile_definitions(perfTest_CRC32_bitShiftLsb PUBLIC CRC_TYPE=CRC32)    
  
add_test(NAME perfTest_CRC7_table COMMAND perfTest_CRC7_table)
add_test(NAME perfTest_CRC7_tableLsb COMMAND perfTest_CRC7_tableLsb)
add_test(NAME perfTest_CRC7_bitShift COMMAND perfTest_CRC7_bitShift)
add_test(NAME perfTest_CRC7_bitShiftLsb COMMAND perfTest_CRC7_bitShiftLsb)
add_test(NAME perfTest_CRC8_table COMMAND perfTest_CRC8_table)
add_test(NAME perfTest_CRC8_bitShift COMMAND perfTest_CRC8_bitShift)
add_test(NAME perfTest_CRC8_bitShiftLsb COMMAND perfTest_CRC8_bitShiftLsb)
add_test(NAME perfTest_CRC16_table COMMAND perfTest_CRC16_table)
add_test(NAME perfTest_CRC16_bitShift COMMAND perfTest_CRC16_bitShift)
add_test(NAME perfTest_CRC16_bitShiftLsb COMMAND perfTest_CRC16_bitShiftLsb)
add_test(NAME perfTest_CRC32_table COMMAND perfTest_CRC32_table)
add_test(NAME perfTest_CRC32_bitShift COMMAND perfTest_CRC32_bitShift)
add_test(NAME perfTest_CRC32_bitShiftLsb COMMAND perfTest_CRC32_bitShiftLsb)


