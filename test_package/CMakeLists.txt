cmake_minimum_required(VERSION 3.15)
project(test_package LANGUAGES C)

find_package(ccstring REQUIRED CONFIG)

add_executable(test_package src/example.c)
target_link_libraries(test_package PRIVATE ccstring::ccstring)
