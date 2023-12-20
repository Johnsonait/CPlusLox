# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Declare the project and set it to a variable
set(PROJECT_NAME CPlusLox)
set(EXECUTABLE_NAME Lox)
project(${PROJECT_NAME})

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Create a variable with all the source files
file(GLOB SOURCES "src/*.cpp" "main.cpp")

# Add an executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Set the output directory for the executable
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)