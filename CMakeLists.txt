cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project("beamng-protocols")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(beamng-protocols ${SOURCES})