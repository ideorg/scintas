mkdir cpp-ipc/build
cd cpp-ipc/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
cp bin/libipc.so ../..
cd ../..
