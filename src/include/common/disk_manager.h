#pragma once

#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#include "common/page.h"

namespace diskv {

class DiskManager {
public:
    explicit DiskManager(std::string disk_name);
    ~DiskManager();
    auto GetNextPageId() const -> int;
    auto FetchPage(size_t page_id) -> Page *;
    void UnpinPage(size_t page_id, Page *page, bool is_dirty);
    auto NewPage(size_t *page_id) -> Page *;

private:
    int disk_fd_;
    int next_page_id_;
    std::vector<Page *> page_pool_;

    auto AcquirePage() -> Page * {
        if (page_pool_.empty()) {
            std::cout << "Page pool has been exhausted." << std::endl;
            exit(1);
        }
        Page *p = page_pool_.back();
        page_pool_.pop_back();
        return p;
    }

    void ReleasePage(Page *p) {
        if (p) {
            page_pool_.emplace_back(p);
        }
    }
};

}
