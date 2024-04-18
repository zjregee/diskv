#include "leveldb/leveldb.h"

namespace diskv {

LevelDB::LevelDB(const std::string &database_name) {
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, database_name, &db_);
    if (!status.ok()) {
        std::cout << "leveldbadaptor database_name -" << database_name << " :leveldb must open success." << std::endl;
        exit(1);
    }
}

LevelDB::~LevelDB() {
    delete db_;
}

bool LevelDB::Insert(const std::string &key, const std::string &value) {
    leveldb::WriteOptions write_options;
    leveldb::Status status = db_->Put(write_options, key, value);
    return status.ok();
}

bool LevelDB::GetValue(const std::string &key, std::string &value) {
    leveldb::ReadOptions read_options;
    leveldb::Status status = db_->Get(read_options, key, &value);
    return status.ok();
}

bool LevelDB::Remove(const std::string &key) {
    leveldb::WriteOptions write_options;
    write_options.sync = true;
    leveldb::Status status = db_->Delete(write_options, key);
    return status.ok();
}

bool LevelDB::GetRange(const std::string &start_key, const std::string &end_key, std::vector<std::pair<std::string, std::string>> &key_value_pair_list) {
    key_value_pair_list.clear();
    leveldb::ReadOptions read_options;
    leveldb::Iterator* it = db_->NewIterator(read_options);
    for (it->Seek(start_key); it->Valid() && it->key().ToString() < end_key; it->Next()) {
        std::string key = it->key().ToString();
        std::string value = it->value().ToString();
        key_value_pair_list.push_back(std::make_pair(key, value));
    }
    leveldb::Status status = it->status();
    delete it;
    return status.ok();
}

}
