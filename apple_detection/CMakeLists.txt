cmake_minimum_required(VERSION 3.17)
project(boundry)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(boundry main.cpp)
target_link_libraries(boundry ${OpenCV_LIBS})