cmake_minimum_required(VERSION 3.23)
project(kvdb)

set(CMAKE_CXX_STANDARD 20)

add_executable(kvdb main.cpp db/database.h db/kvdb.h utils/status.h utils/status.cpp)
