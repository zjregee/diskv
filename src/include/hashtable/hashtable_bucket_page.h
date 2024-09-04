#pragma once

#include <vector>
#include <algorithm>

#include "hashtable/hashtable_page.h"

namespace diskv {

#define BUCKET_PAGE_KEY_TYPE GenericKey<32>
#define BUCKET_PAGE_VALUE_TYPE GenericValue<172>
#define BUCKET_PAGE_KEY_COMPARATOR GenericComparator<32>
#define BUCKET_PAGE_MAPPING_TYPE std::pair<BUCKET_PAGE_KEY_TYPE, BUCKET_PAGE_VALUE_TYPE>
#define BUCKET_PAGE_HEADER_SIZE HASHTABLE_PAGE_HEADER_SIZE
#define BUCKET_PAGE_SIZE ((4096 - BUCKET_PAGE_HEADER_SIZE) / sizeof(BUCKET_PAGE_MAPPING_TYPE))

class HashTableBucketPage : public HashTablePage {
public:
    auto KeyAt(int index) const -> BUCKET_PAGE_KEY_TYPE;
    void SetKeyAt(int index, const BUCKET_PAGE_KEY_TYPE &key);
    auto ValueAt(int index) const -> BUCKET_PAGE_VALUE_TYPE;
    void SetValueAt(int index, const BUCKET_PAGE_VALUE_TYPE &value);
    auto Remove(std::function<bool(const BUCKET_PAGE_MAPPING_TYPE &)> condition) -> bool;
    void RemoveAndSave(std::function<bool(const BUCKET_PAGE_MAPPING_TYPE &)> condition, std::vector<BUCKET_PAGE_MAPPING_TYPE> &removed_array);

private:
    BUCKET_PAGE_MAPPING_TYPE array_[1];
};

}
