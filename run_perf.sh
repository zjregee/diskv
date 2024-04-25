./make_disk.sh sim_disk
rm perf.data
rm output.svg
sudo sysctl -w kernel.perf_event_paranoid=-1
perf record -g ./build/benchmark/diskv_hashtable_benchmark 1000000 sim_disk
perf script | stackcollapse-perf.pl | flamegraph.pl > output.svg
