#pragma once

#include "common/common.h"

namespace diskv {

#define HASHTABLE_PAGE_HEADER_SIZE sizeof(HashTablePage)

class HashTablePage {
public:
    auto GetSize() const -> size_t;
    void SetSize(size_t size);

private:
    size_t size_;
};

}
