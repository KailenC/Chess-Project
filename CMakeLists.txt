cmake_minimum_required(VERSION 4.3)
project(Chess_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(engine/include)

add_executable(Chess_Project
        engine/include/board.h
        engine/include/evaluation.h
        engine/include/search.h
        engine/src/board.cpp
        engine/src/evaluation.cpp
        engine/src/main.cpp
        engine/src/search.cpp)
