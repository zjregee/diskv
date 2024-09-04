cmake_minimum_required(VERSION 3.16.3)

project(diskv CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DISKV_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(DISKV_THIRD_PARTY_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/third_party
    ${PROJECT_SOURCE_DIR}/third_party/leveldb/include
)

include_directories(${DISKV_SRC_INCLUDE_DIR} ${DISKV_THIRD_PARTY_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(benchmark)
add_subdirectory(third_party)
