cmake_minimum_required(VERSION 3.28)
project(bookstore)

set(CMAKE_CXX_STANDARD 20)

add_executable(code
        database.h
        main.cpp
        user.h
        cmd.h
        tools.h
        book.h
        log.h)
