rm -rf /home/zjregee/Code/diskv/test
./build/benchmark/leveldb_benchmark 1000000 32 172 /home/zjregee/Code/diskv/test

sudo ./make_disk.sh /dev/nvme1n1
sudo ./build/benchmark/diskv_hashtable_benchmark 1000000 /dev/nvme1n1

./make_disk.sh test_disk
./build/benchmark/diskv_hashtable_benchmark 1000000 test_disk
