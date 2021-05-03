include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.7.3)
  
FetchContent_Declare(microhal
  GIT_REPOSITORY https://github.com/microHAL/microhal.git
  GIT_TAG 6a048983ce3df563f3dce35b87dea7859fd4ef1b)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_MakeAvailable(microhal)

set(SRCS   
    test.cpp   
    )

add_executable(multiplatformTest
    ${SRCS})
target_link_libraries(multiplatformTest
    microhal-crc    
    nlohmann_json::nlohmann_json
    )
    
add_test(NAME multiplatformTest COMMAND multiplatformTest)

