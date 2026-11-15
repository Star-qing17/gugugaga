cmake_minimum_required(VERSION 3.5)
project(calib_demo)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

add_executable(calib_demo src/main.cpp)
target_link_libraries(calib_demo ${OpenCV_LIBS})
