add_subdirectory(common)
add_subdirectory(leveldb)
add_subdirectory(bplustree)

add_library(diskv STATIC ${ALL_OBJECT_FILES})

set(DISKV_LIBS
    diskv_common
    diskv_leveldb
    diskv_bplustree
)

set(DISKV_THIRD_PARTY_LIBS
    leveldb
    murmur3
)

target_link_libraries(diskv ${DISKV_LIBS} ${DISKV_THIRD_PARTY_LIBS})

target_include_directories(
    diskv PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
