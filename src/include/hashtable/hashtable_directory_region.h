#pragma once

#include "common/disk_manager.h"
#include "hashtable/hashtable_directory_page.h"

namespace diskv {

class HashTableDirectoryRegion {
public:
    explicit HashTableDirectoryRegion(DiskManager *disk_manager);
    auto GetGlobalDepth() -> size_t;
    void SetGlobalDepth(size_t global_depth);
    auto GetBucketLocalDepth(int index) -> size_t;
    void SetBucketLocalDepth(int index, size_t local_depth);
    auto GetBucketPageId(int index) -> size_t;
    void SetBucketPageId(int index, size_t page_id);

private:
    DiskManager* disk_manager_;
};

}
