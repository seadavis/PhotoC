cmake_minimum_required(VERSION 3.16)

project(field_of_view VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )
find_package(OpenCV REQUIRED )


add_subdirectory(./camera/src)
add_subdirectory(./ui/src)

add_executable(field_of_view
    main.cpp
)


target_link_libraries(field_of_view PUBLIC camera ui Qt5::Widgets ${OpenCV_LIBS} -lgphoto2 -lgphoto2_port)
target_include_directories(field_of_view PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/camera/src/"
                          "${PROJECT_SOURCE_DIR}/ui/src/"
                          )

