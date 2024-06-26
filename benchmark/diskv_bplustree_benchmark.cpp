#include <vector>
#include <chrono>
#include <random>
#include <utility>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "bplustree/bplustree.h"
#include "common/disk_manager.h"

auto generate_random_data(size_t data_num, size_t key_size) -> std::vector<std::pair<std::string, size_t>> {
    std::vector<std::pair<std::string, size_t>> random_data;
    random_data.reserve(data_num);

    const std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution_1(0, charset.size() - 1);
    std::uniform_int_distribution<size_t> distribution_2(0, std::numeric_limits<size_t>::max());

    std::unordered_map<std::string, bool> random_keys_map;
    for (size_t i = 0; i < data_num; i++) {
        while (true) {
            std::string random_key;
            random_key.reserve(key_size);
            for (size_t j = 0; j < key_size; j++) {
                random_key.push_back(charset[distribution_1(generator)]);
            }
            if (!random_keys_map.count(random_key)) {
                random_data.push_back(std::make_pair(random_key, distribution_2(generator)));
                random_keys_map[random_key] = true;
                break;
            }
        }
    }

    return random_data;
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cout << "invalid argv num" << std::endl;
        exit(1);
    }

    size_t data_num = std::atoi(argv[1]);
    size_t key_size = std::atoi(argv[2]);
    size_t value_size = std::atoi(argv[3]);
    std::string disk_name = argv[4];

    std::vector<std::pair<std::string, size_t>> data = generate_random_data(data_num, key_size);

    diskv::DiskManager *disk = new diskv::DiskManager(disk_name);
    diskv::BPlusTree *index = new diskv::BPlusTree(disk);

    {
        auto start_time = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < data_num; i++) {
            diskv::KeyType key = diskv::KeyType{};
            std::memcpy(key.data_, data[i].first.c_str(), key_size);
            if (!index->Insert(key, data[i].second)) {
                std::cout << "BPlusTree execute " << i << "th insert error: can' insert key" << std::endl;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "BPlusTree insert duration: " << duration.count() << "ms" << std::endl;
    }

    {
        auto start_time = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < data_num; i++) {
            diskv::KeyType key = diskv::KeyType{};
            std::memcpy(key.data_, data[i].first.c_str(), key_size);
            size_t value;
            if (!index->GetValue(key, &value)) {
                std::cout << "BPlusTree execute " << i << "th queury error: can' find key" << std::endl;
                break;
            }
            if (value != data[i].second) {
                std::cout << "BPlusTree execute " << i << "th queury error: correct value - " <<  data[i].second << " get value - " << value << std::endl;
                break;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "BPlusTree query duration: " << duration.count() << "ms" << std::endl;
    }

    std::cout << "BPlusTree disk usage: " << disk->GetNextPageId() * 4096 / 1024 / 1024 << "MB" << std::endl;

    return 0;
}
