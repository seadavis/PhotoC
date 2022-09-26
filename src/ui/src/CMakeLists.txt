add_library(ui window.cpp)
target_link_libraries(ui PRIVATE camera Qt5::Widgets)
target_include_directories(ui PUBLIC ../../camera/src/)