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

}
