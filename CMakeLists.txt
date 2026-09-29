cmake_minimum_required(VERSION 3.20)

project(ChessLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ChessLib SHARED
    src/Analyzer.cpp
    src/DELETE_LATER.cpp
)

target_include_directories(ChessLib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)