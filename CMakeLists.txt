cmake_minimum_required(VERSION 3.28)
project(bookstore)

set(CMAKE_CXX_STANDARD 14)

add_executable(bookstore
        database.h
        main.cpp)
