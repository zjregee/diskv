#include "hashtable/hashtable_page.h"

namespace diskv {

auto HashTablePage::GetSize() const -> size_t {
    return size_;
}

void HashTablePage::SetSize(size_t size) {
    size_ = size;
}

}
