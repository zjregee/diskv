#include "hashtable/hashtable_bucket_page.h"

namespace diskv {

auto HashTableBucketPage::KeyAt(int index) const -> BUCKET_PAGE_KEY_TYPE {
    return array_[index].first;
}

void HashTableBucketPage::SetKeyAt(int index, const BUCKET_PAGE_KEY_TYPE &key) {
    array_[index].first = key;
}

auto HashTableBucketPage::ValueAt(int index) const -> BUCKET_PAGE_VALUE_TYPE {
    return array_[index].second;
}

void HashTableBucketPage::SetValueAt(int index, const BUCKET_PAGE_VALUE_TYPE &value) {
    array_[index].second = value;
}

void HashTableBucketPage::RemoveAndSave(std::function<bool(const BUCKET_PAGE_MAPPING_TYPE &)> condition, std::vector<BUCKET_PAGE_MAPPING_TYPE> &removed_array) {
    BUCKET_PAGE_MAPPING_TYPE *new_end = std::remove_if(array_, array_ + GetSize(), [&](const BUCKET_PAGE_MAPPING_TYPE &entry) {
        if (condition(entry)) {
            removed_array.emplace_back(entry);
            return true;
        }
        return false;
    });
    SetSize(new_end - array_);
}

}
