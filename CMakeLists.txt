cmake_minimum_required(VERSION 3.10)
project(HighwaySimulation)

# For -O2 flag
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

# Create executables directly in build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})

# Specify where to find headers
include_directories(include)

# Shared source files
set(BASE_SOURCES
    src/Highway.cpp
    src/Vehicle.cpp
)

# First .exe
add_executable(GenerateSimulation 
    src/GenerateSimulation.cpp 
    src/Simulator.cpp
    ${BASE_SOURCES}
)

# Second .exe
add_executable(StartTutor 
    src/StartTutor.cpp 
    src/Tutor.cpp
    ${BASE_SOURCES}
)