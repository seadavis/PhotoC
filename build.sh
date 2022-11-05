cmake -DCMAKE_BUILD_TYPE=Debug -S./src/ -B./build/ 
cmake --build build
./bin/unit_tests
#cd ../
#./bin/field_of_view