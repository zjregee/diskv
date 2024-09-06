#include "hashtable/hashtable.h"

namespace diskv {

auto Hash(const BUCKET_PAGE_KEY_TYPE &key) -> size_t {
    return murmur3::MurmurHash3_x64_128(reinterpret_cast<const void *>(key.data_), KEY_SIZE, 0);
}

auto GetHighBit(int index)-> int {
    return 1 << static_cast<int>(log2(index));
}

auto HashTable::GetBucketNum() -> int {
    size_t global_depth = directory_region_->GetGlobalDepth();
    return static_cast<int>(pow(2, global_depth));
}

auto HashTable::IndexOf(size_t hash)-> int {
    size_t global_depth = directory_region_->GetGlobalDepth();
    int mask = (1 << global_depth) - 1;
    return hash & mask;
}

auto HashTable::RawIndexOf(size_t hash, int index) -> int {
    size_t local_depth = directory_region_->GetBucketLocalDepth(index);
    int mask = (1 << local_depth) - 1;
    return hash & mask;
}

HashTable::HashTable(DiskManager *disk_manager, int max_depth) {
    bucket_region_size_ = static_cast<int>(pow(2, max_depth));
    directory_region_size_ = (bucket_region_size_ - 1) / DIRECTORY_PAGE_SIZE + 1;
    disk_manager_ = disk_manager;
    comparator_ = BUCKET_PAGE_KEY_COMPARATOR();
    directory_region_ = new HashTableDirectoryRegion(disk_manager);
}

auto HashTable::Insert(const BUCKET_PAGE_KEY_TYPE &key, const BUCKET_PAGE_VALUE_TYPE &value) -> bool {
    size_t hash = Hash(key);
    int index = IndexOf(hash);
    int raw_index = RawIndexOf(hash, index);
    auto *raw_page = disk_manager_->FetchPage(directory_region_size_ + raw_index);
    auto *page = reinterpret_cast<HashTableBucketPage *>(raw_page->GetData());
    if (BUCKET_PAGE_SIZE > page->GetSize()) {
        page->SetKeyAt(page->GetSize(), key);
        page->SetValueAt(page->GetSize(), value);
        page->SetSize(page->GetSize() + 1);
        disk_manager_->UnpinPage(directory_region_size_ + raw_index, raw_page, true);
        return true;
    }
    size_t local_depth = directory_region_->GetBucketLocalDepth(raw_index);
    if (local_depth == directory_region_->GetGlobalDepth()) {
        int origin_size = GetBucketNum();
        directory_region_->SetGlobalDepth(directory_region_->GetGlobalDepth() + 1);
        for (int i = origin_size; i < GetBucketNum(); i++) {
            int j = i - GetHighBit(i);
            directory_region_->SetBucketLocalDepth(i, directory_region_->GetBucketLocalDepth(j));
        }
    }
    local_depth += 1;
    int mask = 1 << (local_depth - 1);
    int split_index = raw_index | mask;
    auto *raw_split_page = disk_manager_->FetchPage(directory_region_size_ + split_index);
    auto *split_page = reinterpret_cast<HashTableBucketPage *>(raw_split_page->GetData());
    std::vector<BUCKET_PAGE_MAPPING_TYPE> split_array;
    int target_mask = (1 << local_depth) - 1;
    int target_index = split_index;
    auto condition = [&](const BUCKET_PAGE_MAPPING_TYPE& entry) {
        size_t hash = Hash(entry.first);
        int index = hash & target_mask;
        return index == target_index;
    };
    page->RemoveAndSave(condition, split_array);
    if (split_array.empty()) {
        perror("split array is empty, not expected behavior");
        exit(1);
    }
    split_page->SetSize(split_array.size());
    for (int i = 0; i < split_array.size(); i++) {
        split_page->SetKeyAt(i, split_array[i].first);
        split_page->SetValueAt(i, split_array[i].second);
    }
    int key_index = hash & target_mask;
    if (key_index == raw_index) {
        page->SetKeyAt(page->GetSize(), key);
        page->SetValueAt(page->GetSize(), value);
        page->SetSize(page->GetSize() + 1);
    } else {
        split_page->SetKeyAt(split_page->GetSize(), key);
        split_page->SetValueAt(split_page->GetSize(), value);
        split_page->SetSize(split_page->GetSize() + 1);
    }
    int diff = 1 << local_depth;
    for (int i = raw_index; i < GetBucketNum(); i += diff) {
        directory_region_->SetBucketLocalDepth(i, local_depth);
    }
    for (int i = split_index; i < GetBucketNum(); i += diff) {
        directory_region_->SetBucketLocalDepth(i, local_depth);
    }
    disk_manager_->UnpinPage(directory_region_size_ + raw_index, raw_page, true);
    disk_manager_->UnpinPage(directory_region_size_ + split_index, raw_split_page, true);
    return true;
}

auto HashTable::GetValue(const BUCKET_PAGE_KEY_TYPE &key, BUCKET_PAGE_VALUE_TYPE *value) -> bool {
    size_t hash = Hash(key);
    int index = IndexOf(hash);
    int raw_index = RawIndexOf(hash, index);
    auto *raw_page = disk_manager_->FetchPage(directory_region_size_ + raw_index);
    auto *page = reinterpret_cast<HashTableBucketPage *>(raw_page->GetData());
    for (int i = 0; i < page->GetSize(); i++) {
        if (comparator_(key, page->KeyAt(i)) == 0) {
            *value = page->ValueAt(i);
            disk_manager_->UnpinPage(directory_region_size_ + raw_index, raw_page, false);
            return true;
        }
    }
    disk_manager_->UnpinPage(directory_region_size_ + raw_index, raw_page, false);
    return false;
}

auto HashTable::Remove(const BUCKET_PAGE_KEY_TYPE &key) -> bool {
    size_t hash = Hash(key);
    int index = IndexOf(hash);
    int raw_index = RawIndexOf(hash, index);
    auto *raw_page = disk_manager_->FetchPage(directory_region_size_ + raw_index);
    auto *page = reinterpret_cast<HashTableBucketPage *>(raw_page->GetData());
    auto condition = [&](const BUCKET_PAGE_MAPPING_TYPE& entry) {
        return comparator_(key, entry.first) == 0;
    };
    bool removed = page->Remove(condition);
    disk_manager_->UnpinPage(directory_region_size_ + raw_index, raw_page, true);
    return removed;
}

void HashTable::PrintDiskUsage() {
    std::cout << "HashTable disk usage: " << (directory_region_size_ + bucket_region_size_) * PAGE_SIZE / 1024 / 1024 << "MB" << std::endl;
    std::cout << "HashTable bucket usage: total bucket num - " << bucket_region_size_ << " used bucket num - " << GetBucketNum() << std::endl;
}

void HashTable::PrintInternal() {
    std::cout << "[directory region]" << std::endl;
    std::cout << "global_depth: " << directory_region_->GetGlobalDepth() << std::endl;
    std::cout << "[bucket region]" << std::endl;
    for (int i = 0; i < GetBucketNum(); i++) {
        auto *raw_page = disk_manager_->FetchPage(directory_region_size_ + i);
        auto *page = reinterpret_cast<HashTableBucketPage *>(raw_page->GetData());
        std::cout << "bucket " << i << ": size - " << page->GetSize() << std::endl;
        std::cout << "bucket " << i << ": local_depth - " << directory_region_->GetBucketLocalDepth(i) << std::endl;
        for (int j = 0; j < page->GetSize(); j++) {
            std::cout << "bucket " << i << ": key - " << std::string(page->KeyAt(j).data_, KEY_SIZE) << " value - " << std::string(page->ValueAt(j).data_, VALUE_SIZE) << std::endl;
        }
        disk_manager_->UnpinPage(directory_region_size_ + i, raw_page, false);
    }
}

}
