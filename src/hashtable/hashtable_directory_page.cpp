#include "hashtable/hashtable_directory_page.h"

namespace diskv {

auto HashTableDirectoryPage::GetGlobalDepth() -> size_t {
    return global_depth_;
}

void HashTableDirectoryPage::SetGlobalDepth(size_t global_depth) {
    global_depth_ = global_depth;
}

auto HashTableDirectoryPage::GetBucketLocalDepth(int index) -> size_t {
    return array_[index].first;
}

void HashTableDirectoryPage::SetBucketLocalDepth(int index, size_t local_depth) {
    array_[index].first = local_depth;
}

auto HashTableDirectoryPage::GetBucketPageId(int index) -> size_t {
    return array_[index].second;
}

void HashTableDirectoryPage::SetBucketPageId(int index, size_t page_id) {
    array_[index].second = page_id;
}

}
