add_library(
    diskv_bplustree
    OBJECT
    bplustree.cpp
    bplustree_page.cpp
    bplustree_leaf_page.cpp
    bplustree_internal_page.cpp
)

set(ALL_OBJECT_FILES ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:diskv_bplustree> PARENT_SCOPE)
