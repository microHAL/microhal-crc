include(FetchContent)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY "https://github.com/onqtam/doctest"
        GIT_TAG "932a2ca50666138256dae56fbb16db3b1cae133a"
        )

FetchContent_MakeAvailable(doctest)

set(SRCS
    main.cpp
    test_predefinedCrc.cpp
    test_crcUsedInSdCard.cpp
    test_tableGeneration.cpp
    )

add_executable(unitTests_bitShiftMsb ${SRCS})
target_link_libraries(unitTests_bitShiftMsb microhal-crc doctest)
target_compile_definitions(unitTests_bitShiftMsb PRIVATE CRC_IMPLEMENTATION=BitShift)

add_executable(unitTests_bitShiftLsb ${SRCS})
target_link_libraries(unitTests_bitShiftLsb microhal-crc doctest)
target_compile_definitions(unitTests_bitShiftLsb PRIVATE CRC_IMPLEMENTATION=BitShiftLsb)

add_executable(unitTests_table256Msb ${SRCS})
target_link_libraries(unitTests_table256Msb microhal-crc doctest)
target_compile_definitions(unitTests_table256Msb PRIVATE CRC_IMPLEMENTATION=Table256)

add_executable(unitTests_table256Lsb ${SRCS})
target_link_libraries(unitTests_table256Lsb microhal-crc doctest)
target_compile_definitions(unitTests_table256Lsb PRIVATE CRC_IMPLEMENTATION=Table256Lsb)

add_test(NAME unitTests_bitShiftMsb COMMAND unitTests_bitShiftMsb)
add_test(NAME unitTests_bitShiftLsb COMMAND unitTests_bitShiftLsb)
add_test(NAME unitTests_table256Msb COMMAND unitTests_table256Msb)
add_test(NAME unitTests_table256Lsb COMMAND unitTests_table256Lsb)

