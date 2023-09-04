cmake_minimum_required(VERSION 3.24)
project(DoorRiddle)

set(CMAKE_CXX_STANDARD 23)

# Door riddle
add_executable(DoorRiddle DoorRiddle/DoorRiddle.cpp)
add_executable(DoorRiddleJenda DoorRiddle/DoorRiddleJenda.cpp)
