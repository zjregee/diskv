#include "common/page.h"

namespace diskv {

Page::Page() {
    data_ = new char[PAGE_SIZE];
}

Page::~Page() {
    delete[] data_;
}

auto Page::GetData() -> char* {
    return data_;
}

void Page::ResetMemory() {
    memset(data_, 0, PAGE_SIZE);
}

}
