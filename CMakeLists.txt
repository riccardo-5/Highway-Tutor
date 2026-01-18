cmake_minimum_required(VERSION 3.10)
project(HighwaySimulation)

# For -O2 flag
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

#Default build type (if not specified): Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

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

# GenerateSimulation.exe
add_executable(GenerateSimulation 
    src/GenerateSimulation.cpp 
    src/Simulator.cpp
    ${BASE_SOURCES}
)

# StartTutor.exe
add_executable(StartTutor 
    src/StartTutor.cpp 
    src/Tutor.cpp
    ${BASE_SOURCES}
)