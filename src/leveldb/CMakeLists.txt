add_library(
    diskv_leveldb
    OBJECT
    leveldb.cpp
)

set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:diskv_leveldb> PARENT_SCOPE)
