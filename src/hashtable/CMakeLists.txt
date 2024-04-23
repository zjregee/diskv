add_library(
    diskv_hashtable
    OBJECT
    hashtable.cpp
    hashtable_page.cpp
    hashtable_bucket_page.cpp
    hashtable_directory_page.cpp
    hashtable_directory_region.cpp
)

set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:diskv_hashtable> PARENT_SCOPE)
