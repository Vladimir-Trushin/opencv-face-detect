cmake_minimum_required(VERSION 3.22)
project(face_detect CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(face_detect src/main.cpp)
target_link_libraries(face_detect ${OpenCV_LIBS})
target_include_directories(face_detect PRIVATE ${OpenCV_INCLUDE_DIRS})