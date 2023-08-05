add_library(camera camera.cpp)
target_link_libraries(camera -lgphoto2 -lgphoto2_port common_tests)
target_include_directories(camera PUBLIC ../../tests/src/)