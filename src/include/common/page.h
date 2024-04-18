#pragma once

#include <cstring>

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
