cmake_minimum_required(VERSION 3.16)

project(field_of_view VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )
find_package(OpenCV REQUIRED )

add_executable(field_of_view
    window.h
    window.cpp
    main.cpp
)


target_link_libraries(field_of_view PRIVATE Qt5::Widgets ${OpenCV_LIBS})

