cmake_minimum_required(VERSION 3.10)
project(HighwaySimulation)

# 1. Specify where to find your header files
include_directories(include)

# 2. Group your shared source files (logic used by both apps)
set(COMMON_SOURCES
    src/Highway.cpp
    src/Simulator.cpp
    src/Tutor.cpp
    src/Vehicle.cpp
)

# 3. Create the first executable: GenerateSimulation
# This uses its specific main file + all shared logic
add_executable(GenerateSimulation 
    src/GenerateSimulation.cpp 
    ${COMMON_SOURCES}
)

# 4. Create the second executable: StartTutor
# This uses its specific main file + all shared logic
add_executable(StartTutor 
    src/StartTutor.cpp 
    ${COMMON_SOURCES}
)