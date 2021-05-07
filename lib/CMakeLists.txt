cmake_minimum_required(VERSION 3.12)

add_library(microhal-crc INTERFACE)
target_include_directories(microhal-crc INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(microhal-crc INTERFACE cxx_std_20)
