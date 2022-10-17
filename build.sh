cmake -S./src/ -B./build/ 
cmake --build build
cd build && ctest
cd ../
./bin/field_of_view