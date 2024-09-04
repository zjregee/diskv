#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "hashtable/hashtable.h"
#include "common/disk_manager.h"

auto generate_random_data(size_t data_num, size_t key_size, size_t value_size) -> std::vector<std::pair<std::string, std::string>> {
    std::vector<std::pair<std::string, std::string>> random_data;
    random_data.reserve(data_num);

    const std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, charset.size() - 1);

    std::unordered_map<std::string, bool> random_keys_map;
    for (size_t i = 0; i < data_num; i++) {
        while (true) {
            std::string random_key;
            random_key.reserve(key_size);
            for (size_t j = 0; j < key_size; j++) {
                random_key.push_back(charset[distribution(generator)]);
            }
            if (!random_keys_map.count(random_key)) {
                std::string random_value;
                random_value.reserve(value_size);
                for (size_t j = 0; j < value_size; j++) {
                    random_value.push_back(charset[distribution(generator)]);
                }
                random_data.push_back(std::make_pair(random_key, random_value));
                random_keys_map[random_key] = true;
                break;
            }
        }
    }

    return random_data;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "invalid argv num" << std::endl;
        exit(1);
    }

    int data_num = std::atoi(argv[1]);
    std::string disk_name = argv[2];

    std::vector<std::pair<std::string, std::string>> data = generate_random_data(data_num, 32, 172);

    diskv::DiskManager *disk = new diskv::DiskManager(disk_name);
    diskv::HashTable *index = new diskv::HashTable(disk, 20);

    {
        auto start_time = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < data_num; i++) {
            diskv::BUCKET_PAGE_KEY_TYPE key = diskv::BUCKET_PAGE_KEY_TYPE();
            diskv::BUCKET_PAGE_VALUE_TYPE value = diskv::BUCKET_PAGE_VALUE_TYPE();
            std::memcpy(key.data_, data[i].first.c_str(), 32);
            std::memcpy(value.data_, data[i].second.c_str(), 172);
            if (!index->Insert(key, value)) {
                std::cout << "HashTable execute " << i << "th insert error: can' insert key" << std::endl;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "HashTable insert duration: " << duration.count() << "ms" << std::endl;
    }

    {
        auto start_time = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < data_num; i++) {
            diskv::BUCKET_PAGE_KEY_TYPE key = diskv::BUCKET_PAGE_KEY_TYPE();
            diskv::BUCKET_PAGE_VALUE_TYPE res_value = diskv::BUCKET_PAGE_VALUE_TYPE();
            std::memcpy(key.data_, data[i].first.c_str(), 32);
            std::memcpy(res_value.data_, data[i].second.c_str(), 172);
            diskv::BUCKET_PAGE_VALUE_TYPE value;
            if (!index->GetValue(key, &value)) {
                std::cout << "HashTable execute " << i << "th queury error: can' find key" << std::endl;
            }
            if (memcmp(value.data_, res_value.data_, 172) != 0) {
                std::cout << "HashTable execute " << i << "th queury error: correct value - " << std::string(res_value.data_, 172) << " get value - " << std::string(value.data_, 172) << std::endl;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "HashTable query duration: " << duration.count() << "ms" << std::endl;
    }

    {
        auto start_time = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < data_num; i++) {
            diskv::BUCKET_PAGE_KEY_TYPE key = diskv::BUCKET_PAGE_KEY_TYPE();
            std::memcpy(key.data_, data[i].first.c_str(), 32);
            if (!index->Remove(key)) {
                std::cout << "HashTable execute " << i << "th remove error: can' remove key" << std::endl;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "HashTable remove duration: " << duration.count() << "ms" << std::endl;
    }

    {
        for (int i = 0; i < data_num; i++) {
            diskv::BUCKET_PAGE_KEY_TYPE key = diskv::BUCKET_PAGE_KEY_TYPE();
            diskv::BUCKET_PAGE_VALUE_TYPE res_value = diskv::BUCKET_PAGE_VALUE_TYPE();
            std::memcpy(key.data_, data[i].first.c_str(), 32);
            std::memcpy(res_value.data_, data[i].second.c_str(), 172);
            diskv::BUCKET_PAGE_VALUE_TYPE value;
            if (index->GetValue(key, &value)) {
                std::cout << "HashTable execute " << i << "th remove error: can' remove key" << std::endl;
            }
        }
    }

    index->PrintDiskUsage();

    return 0;
}
