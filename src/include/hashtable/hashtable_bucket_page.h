#pragma once

#include "hashtable/hashtable_page.h"

namespace diskv {

#define BUCKET_PAGE_KEY_TYPE GenericKey<32>
#define BUCKET_PAGE_VALUE_TYPE GenericValue<32>
#define BUCKET_PAGE_KEY_COMPARATOR GenericComparator<32>
#define BUCKET_PAGE_MAPPING_TYPE std::pair<BUCKET_PAGE_KEY_TYPE, BUCKET_PAGE_VALUE_TYPE>
#define BUCKET_PAGE_HEADER_SIZE (HASHTABLE_PAGE_HEADER_SIZE + sizeof(size_t))
#define BUCKET_PAGE_SIZE ((4096 - BUCKET_PAGE_HEADER_SIZE) / sizeof(BUCKET_PAGE_MAPPING_TYPE))

class HashTableBucketPage  {
public:
    auto KeyAt(int index) const -> BUCKET_PAGE_KEY_TYPE;
    void SetKeyAt(int index, const BUCKET_PAGE_KEY_TYPE &key);
    auto ValueAt(int index) const -> BUCKET_PAGE_VALUE_TYPE;
    void SetValueAt(int index, const BUCKET_PAGE_VALUE_TYPE &value);

private:
    BUCKET_PAGE_MAPPING_TYPE array_[1];
};

}
