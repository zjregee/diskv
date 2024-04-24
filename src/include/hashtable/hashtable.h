#pragma once

#include <cmath>
#include <iostream>
#include <murmur3/MurmurHash3.h>

#include "hashtable/hashtable_bucket_page.h"
#include "hashtable/hashtable_directory_region.h"

namespace diskv {

class HashTable {
public:
    auto Insert(const BUCKET_PAGE_KEY_TYPE &key, const BUCKET_PAGE_VALUE_TYPE &value) -> bool;
    auto GetValue(const BUCKET_PAGE_KEY_TYPE &key, BUCKET_PAGE_VALUE_TYPE *value) -> bool;

private:
    DiskManager *disk_manager_;
    size_t directory_region_size_;
    BUCKET_PAGE_KEY_COMPARATOR comparator_;
    HashTableDirectoryRegion *directory_region_;

    auto GetBucketNum() -> int;
    auto IndexOf(size_t hash) -> int;
    auto RawIndexOf(size_t hash, int index) -> int;
};

}
