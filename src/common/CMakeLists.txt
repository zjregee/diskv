add_library(
    diskv_common
    OBJECT
    page.cpp
    disk_manager.cpp
)

set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:diskv_common> PARENT_SCOPE)
