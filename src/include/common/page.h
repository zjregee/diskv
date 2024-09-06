#pragma once

#include <cstring>

#include "common/config.h"

namespace diskv {

class Page {
public:
    Page();
    ~Page();
    auto GetData() -> char *;
    void ResetMemory();

private:
    char *data_;
};

}
