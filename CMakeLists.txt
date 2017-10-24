cmake_minimum_required(VERSION 3.7)
project(RubiksCube)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS -Wall -Werror)

set(SOURCE_FILES
        main.cpp
        Piece.cpp
        Piece.h
        RubiksCube.cpp
        RubiksCube.h
        Utils.cpp
        Utils.h)
add_executable(RubiksCube ${SOURCE_FILES})