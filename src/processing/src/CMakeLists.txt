add_library(processing processing.cpp canvasmanager.cpp utilities.cpp)
target_link_libraries(processing PRIVATE Eigen3::Eigen Threads::Threads)