# This script builds and does the testing or uns the UI
# pass -s to start the program. Pass -t to run the automated tests
# no arguments just builds


cmake -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_C_FLAGS_DEBUG="-g -O3" \
      -DCMAKE_CXX_FLAGS_DEBUG="-g -O3" \
      -S./src/ -B./build/ 
cmake --build build

if [ "$1" == "-t" ]
then
    ./bin/unit_tests
fi

if [ "$1" == "-s" ]
then
    ./bin/field_of_view $2
fi