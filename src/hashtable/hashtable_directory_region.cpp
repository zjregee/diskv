#include "hashtable/hashtable_directory_region.h"

namespace diskv {

auto HashTableDirectoryRegion::GetGlobalDepth() -> size_t {
    auto *raw_page = disk_manager_->FetchPage(0);
    auto *page = reinterpret_cast<HashTableDirectoryPage *>(raw_page->GetData());
    size_t global_depth = page->GetGlobalDepth();
    disk_manager_->UnpinPage(0, raw_page, false);
    return global_depth;
}

void HashTableDirectoryRegion::SetGlobalDepth(size_t global_depth) {
    auto *raw_page = disk_manager_->FetchPage(0);
    auto *page = reinterpret_cast<HashTableDirectoryPage *>(raw_page->GetData());
    page->SetGlobalDepth(global_depth);
    disk_manager_->UnpinPage(0, raw_page, true);
}

auto HashTableDirectoryRegion::GetBucketLocalDepth(int index) -> size_t {
    int pos = index / DIRECTORY_PAGE_SIZE;
    int off = index % DIRECTORY_PAGE_SIZE;
    auto *raw_page = disk_manager_->FetchPage(pos);
    auto *page = reinterpret_cast<HashTableDirectoryPage *>(raw_page->GetData());
    size_t local_depth = page->GetBucketLocalDepth(off);
    disk_manager_->UnpinPage(pos, raw_page, false);
    return local_depth;
}

void HashTableDirectoryRegion::SetBucketLocalDepth(int index, size_t local_depth) {
    int pos = index / DIRECTORY_PAGE_SIZE;
    int off = index % DIRECTORY_PAGE_SIZE;
    auto *raw_page = disk_manager_->FetchPage(pos);
    auto *page = reinterpret_cast<HashTableDirectoryPage *>(raw_page->GetData());
    page->SetBucketLocalDepth(off, local_depth);
    disk_manager_->UnpinPage(pos, raw_page, true);
}

auto HashTableDirectoryRegion::GetBucketPageId(int index) -> size_t {
    int pos = index / DIRECTORY_PAGE_SIZE;
    int off = index % DIRECTORY_PAGE_SIZE;
    auto *raw_page = disk_manager_->FetchPage(pos);
    auto *page = reinterpret_cast<HashTableDirectoryPage *>(raw_page->GetData());
    size_t local_depth = page->GetBucketPageId(off);
    disk_manager_->UnpinPage(pos, raw_page, false);
    return local_depth;
}

void HashTableDirectoryRegion::SetBucketPageId(int index, size_t page_id) {
    int pos = index / DIRECTORY_PAGE_SIZE;
    int off = index % DIRECTORY_PAGE_SIZE;
    auto *raw_page = disk_manager_->FetchPage(pos);
    auto *page = reinterpret_cast<HashTableDirectoryPage *>(raw_page->GetData());
    page->SetBucketPageId(off, page_id);
    disk_manager_->UnpinPage(pos, raw_page, true);
}

}
