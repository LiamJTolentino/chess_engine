cmake_minimum_required(VERSION 3.20)

project(ChessLib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ChessLib SHARED
    src/Analyzer.cpp
    src/DELETE_LATER.cpp
    src/ChessFunctions.cpp
    src/BoardSquare.cpp
)

target_include_directories(ChessLib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

find_package(Doxygen REQUIRED)

doxygen_add_docs(
    docs
    ${PROJECT_SOURCE_DIR}/include
    COMMENT "Generating API documentation"
)