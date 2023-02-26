# This script builds and does the testing or uns the UI
# pass -s to start the program. Pass -t to run the automated tests
# no arguments just builds


cmake -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_C_FLAGS_DEBUG="-g -O3" \
      -DCMAKE_CXX_FLAGS_DEBUG="-g -O3" \
      -DHPX_DIR=/usr/include/hpx/lib/cmake/HPX/ \
      -DHPX_WITH_MALLOC=system \
      -DTCMALLOC_INCLUDE_DIR=/usr/include/gperftools/include/ \
      -DTCMALLOC_LIBRARY=/usr/include/gperftools/lib/libtcmalloc_minimal_debug.so \
      -DHWLOC_LIBRARY=/usr/include/hwloc/lib/libhwloc.so \
      -DHWLOC_INCLUDE_DIR=/usr/include/hwloc/include/ \
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