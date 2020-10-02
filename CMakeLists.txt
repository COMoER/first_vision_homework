cmake_minimum_required(VERSION 3.17)
project(demo1)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(demo1 main.cpp)

target_link_libraries(demo1 Eigen3::Eigen)
target_link_libraries(demo1 ${OpenCV_LIBS})





