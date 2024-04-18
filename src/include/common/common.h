#pragma once

#include <vector>
#include <cstring>
#include <stddef.h>

namespace diskv {

template <size_t KeySize>
class GenericKey {
public:
    char data_[KeySize];
};

template <size_t ValueSize>
class GenericValue {
public:
    char data_[ValueSize];
};

template <size_t KeySize>
class GenericComparator {
public:
    inline auto operator()(const GenericKey<KeySize> &lhs, const GenericKey<KeySize> &rhs) const -> int {
        return std::memcmp(lhs.data_, rhs.data_, KeySize);
    }
};

}
