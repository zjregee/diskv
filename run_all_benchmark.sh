# rm -rf /home/zjregee/nvme/test
# ./build/benchmark/leveldb_benchmark 1000000 32 172 /home/zjregee/nvme/test

sudo ./make_disk.sh /dev/nvme1n1
sudo ./build/benchmark/diskv_hashtable_benchmark 1000000 /dev/nvme1n1
