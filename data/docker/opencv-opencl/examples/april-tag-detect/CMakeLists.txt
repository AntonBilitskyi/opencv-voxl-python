cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(AprilTagDetect)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(TagDetect april-tag-detect.cpp)

target_link_libraries(TagDetect ${OpenCV_LIBS})
