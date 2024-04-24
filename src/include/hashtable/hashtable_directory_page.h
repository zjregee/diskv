#pragma once

#include <utility>

#include "hashtable/hashtable_page.h"

namespace diskv {

#define DIRECTORY_PAGE_MAPPING_TYPE std::pair<size_t, size_t>
#define DIRECTORY_PAGE_HEADER_SIZE (HASHTABLE_PAGE_HEADER_SIZE + sizeof(size_t))
#define DIRECTORY_PAGE_SIZE ((4096 - DIRECTORY_PAGE_HEADER_SIZE) / sizeof(DIRECTORY_PAGE_MAPPING_TYPE))

class HashTableDirectoryPage : public HashTablePage {
public:
    auto GetGlobalDepth() -> size_t;
    void SetGlobalDepth(size_t global_depth);
    auto GetBucketLocalDepth(int index) -> size_t;
    void SetBucketLocalDepth(int index, size_t local_depth);
    auto GetBucketPageId(int index) -> size_t;
    void SetBucketPageId(int index, size_t page_id);

private:
    size_t global_depth_;
    DIRECTORY_PAGE_MAPPING_TYPE array_[1];
};

}
