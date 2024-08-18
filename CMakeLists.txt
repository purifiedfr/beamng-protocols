cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project("beamng-protocols")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(beamng-protocols ${SOURCES})

if (WIN32)
	target_compile_definitions(beamng-protocols PRIVATE BP_WINDOWS=1 BP_LINUX=0)
elseif(LINUX)
	target_compile_definitions(beamng-protocols PRIVATE BP_WINDOWS=0 BP_LINUX=1)
else()
	message(FATAL_ERROR "Unsupported operating system")
endif()
