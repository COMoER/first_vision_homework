cmake_minimum_required(VERSION 3.17)
project(transposion)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(transposion main.cpp)
target_link_libraries(transposion ${OpenCV_LIBS})