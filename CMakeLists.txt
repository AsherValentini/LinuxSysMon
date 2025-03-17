cmake_minimum_required(VERSION 3.10)

# Project Name and Language
project(LinuxSysMon LANGUAGES CXX)

# Set C++ standard 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories 
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect source files 
set(SOURCES 
	src/main.cpp
)

# Create the executable
add_executable(LinuxSysMon ${SOURCES})

# Include any subdirecties here
# add_subdirectory(src/core)

