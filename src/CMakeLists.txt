cmake_minimum_required(VERSION 3.16)

project(field_of_view VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(RESOURCE_FILES ./ui/src/app_resources.qrc)
set(BENCHMARK_ENABLE_LIBPFM ON)


find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )
find_package(OpenCV REQUIRED )
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(benchmark REQUIRED)
find_package(Threads REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

add_subdirectory(./camera/src)
add_subdirectory(./ui/src)
add_subdirectory(./common/src)
add_subdirectory(./processing/src)
add_subdirectory(./blending/src)
add_subdirectory(./tests/src)

set(SOURCES main.cpp)
qt5_add_resources(SOURCES ./ui/src/app_resources.qrc)
add_executable(field_of_view
    ${SOURCES}
)

target_link_libraries(field_of_view PUBLIC common blending processing camera ui Qt5::Widgets ${OpenCV_LIBS})

target_include_directories(field_of_view PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/camera/src/"
                          "${PROJECT_SOURCE_DIR}/ui/src/"
                          "${PROJECT_SOURCE_DIR}/common/src/"
                          "${PROJECT_SOURCE_DIR}/blending/src/"
                          )

enable_testing()
add_executable(
  unit_tests
  ./processing/tests/canvas_tests.cpp
  ./blending/tests/stacking_tests.cpp
)
target_link_libraries(
  unit_tests
  processing 
  common 
  common_tests
  blending 
  GTest::gtest_main 
  ${OpenCV_LIBS}
)

target_include_directories(unit_tests PUBLIC
                          "${PROJECT_SOURCE_DIR}/processing/src/"
                          "${PROJECT_SOURCE_DIR}/blending/src/"
                          "${PROJECT_SOURCE_DIR}/tests/src/"
)

include(GoogleTest)
gtest_discover_tests(unit_tests)

add_executable(
  benchmarks
  ./benchmarks/blending_benchmarks.cpp
)
target_link_libraries(
  benchmarks
  processing benchmark::benchmark ${OpenCV_LIBS}
)
target_include_directories(benchmarks PUBLIC
                          "${PROJECT_SOURCE_DIR}/processing/src/"
)


