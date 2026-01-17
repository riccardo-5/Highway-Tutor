cmake_minimum_required(VERSION 3.10)
project(HighwaySimulation)

# Imposta lo standard C++ e l'ottimizzazione richiesta [cite: 118, 103]
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

# 1. Specifica dove trovare gli header
include_directories(include)

# 2. File sorgente condivisi (logica base dell'autostrada e veicoli)
set(BASE_SOURCES
    src/Highway.cpp
    src/Vehicle.cpp
)

# 3. Primo eseguibile: Simulatore (Generazione profili e passaggi)
# Non necessita di Tutor.cpp 
add_executable(GenerateSimulation 
    src/GenerateSimulation.cpp 
    src/Simulator.cpp
    ${BASE_SOURCES}
)

# 4. Secondo eseguibile: Sistema Tutor (Analisi transiti e sanzioni)
# Non necessita di Simulator.cpp 
add_executable(StartTutor 
    src/StartTutor.cpp 
    src/Tutor.cpp
    ${BASE_SOURCES}
)