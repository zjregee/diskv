if [ ! -d build ]; then
    mkdir -p build
fi
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . -j8
