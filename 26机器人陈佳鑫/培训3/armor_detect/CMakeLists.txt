cmake_minimum_required(VERSION 3.11)
project(armor_detect)

find_package(OpenCV REQUIRED)

add_executable(ARMOR_DETECT src/main.cpp)
target_link_libraries(ARMOR_DETECT ${OpenCV_LIBS})
