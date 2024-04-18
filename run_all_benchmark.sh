./compile.sh

# rm -rf test_dir
# ./build/benchmark/leveldb_benchmark 1000000 32 4 test_dir
# rm -rf test_dir

./make_disk.sh sim_disk
./build/benchmark/diskv_bplustree_benchmark 10000000 32 4 sim_disk
rm sim_disk
