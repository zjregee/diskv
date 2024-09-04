#include <vector>
#include <chrono>
#include <random>
#include <utility>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "leveldb/leveldb.h"

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
    if (argc < 5) {
        std::cout << "invalid argv num" << std::endl;
        exit(1);
    }

    size_t data_num = std::atoi(argv[1]);
    size_t key_size = std::atoi(argv[2]);
    size_t value_size = std::atoi(argv[3]);
    std::string data_dir = argv[4];

    std::vector<std::pair<std::string, std::string>> data = generate_random_data(data_num, key_size, value_size);

    diskv::LevelDB *index = new diskv::LevelDB(data_dir);

    {
        auto start_time = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < data_num; i++) {
            if (!index->Insert(data[i].first, data[i].second)) {
                std::cout << "LevelDB execute " << i << "th insert error: can' insert key" << std::endl;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "LevelDB insert duration: " << duration.count() << "ms" << std::endl;
    }

    {
        auto start_time = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < data_num; i++) {
            std::string value;
            if (!index->GetValue(data[i].first, value)) {
                std::cout << "LevelDB execute " << i << "th queury error: can' find key" << std::endl;
            }
            if (value != data[i].second) {
                std::cout << "LevelDB execute " << i << "th queury error: correct value - " <<  data[i].second << " get value - " << value << std::endl;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "LevelDB query duration: " << duration.count() << "ms" << std::endl;
    }

    {
        auto start_time = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < data_num; i++) {
            if (!index->Remove(data[i].first)) {
                std::cout << "LevelDB execute " << i << "th remove error: can' remove key" << std::endl;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "LevelDB remove duration: " << duration.count() << "ms" << std::endl;
    }

    {
        for (size_t i = 0; i < data_num; i++) {
            std::string value;
            if (index->GetValue(data[i].first, value)) {
                std::cout << "LevelDB execute " << i << "th queury error: can' remove key" << std::endl;
            }
        }
    }

    return 0;
}
