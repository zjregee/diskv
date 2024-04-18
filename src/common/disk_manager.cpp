#include "common/disk_manager.h"

namespace diskv {

DiskManager::DiskManager(std::string disk_name) {
    next_page_id_ = 0;
    disk_fd_ = open(disk_name.c_str(), O_RDWR);
    if (disk_fd_ == -1) {
        std::cout << "DiskManager internal error" << std::endl;
        exit(1);
    }
    for (size_t i = 0; i < 8; i++) {
        Page *p = new Page();
        page_pool_.emplace_back(p);
    }
}

DiskManager::~DiskManager() {
    if (close(disk_fd_) == -1) {
        std::cout << "DiskManager internal error" << std::endl;
        exit(1);
    }
}

auto DiskManager::GetNextPageId() const -> int {
    return next_page_id_;
}

auto DiskManager::FetchPage(size_t page_id) -> Page * {
    Page *page = AcquirePage();
    page->ResetMemory();
    off_t off = page_id * 4096;
    if (lseek(disk_fd_, off, SEEK_SET) == -1) {
        std::cout << "DiskManager internal error" << std::endl;
        exit(1);
    }
    if (read(disk_fd_, page->GetData(), 4096) == -1) {
        std::cout << "DiskManager internal error" << std::endl;
        exit(1);
    }
    return page;
}

void DiskManager::UnpinPage(size_t page_id, Page *page, bool is_dirty) {
    if (is_dirty) {
        off_t off = page_id * 4096;
        if (lseek(disk_fd_, off, SEEK_SET) == -1) {
            std::cout << "DiskManager internal error" << std::endl;
            exit(1);
        }
        if (write(disk_fd_, page->GetData(), 4096) == -1) {
            std::cout << "DiskManager internal error" << std::endl;
            exit(1);
        }
    }
    ReleasePage(page);
}

auto DiskManager::NewPage(size_t *page_id) -> Page * {
    *page_id = next_page_id_;
    next_page_id_++;
    Page *page = AcquirePage();
    page->ResetMemory();
    off_t off = *page_id * 4096;
    if (lseek(disk_fd_, off, SEEK_SET) == -1) {
        std::cout << "DiskManager internal error" << std::endl;
        exit(1);
    }
    if (write(disk_fd_, page->GetData(), 4096) == -1) {
        std::cout << "DiskManager internal error" << std::endl;
        exit(1);
    }
    return page;
}

}
