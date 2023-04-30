add_library(processing processing.cpp canvasmanager.cpp)
target_link_libraries(processing PRIVATE Eigen3::Eigen Threads::Threads common)
target_include_directories(processing PUBLIC ../../common/src/)