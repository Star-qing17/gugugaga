cmake_minimum_required(VERSION 3.11)
project(hsv_mask)

find_package(OpenCV REQUIRED)

add_executable(HSV_MASK src/main.cpp)
target_link_libraries(HSV_MASK ${OpenCV_LIBS})
