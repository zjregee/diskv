#pragma once

#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <leveldb/db.h>
#include <leveldb/cache.h>
#include <leveldb/options.h>
#include <leveldb/iterator.h>

namespace diskv {

class LevelDB {
public:
    LevelDB(const std::string &database_name);
    ~LevelDB();
    bool Insert(const std::string &key, const std::string &value);
    bool GetValue(const std::string &key, std::string &value);
    bool Remove(const std::string &key);
    bool GetRange(const std::string &start_key, const std::string &end_key, std::vector<std::pair<std::string, std::string>> &key_value_pair_list);

private:
    leveldb::DB* db_;
};

}
