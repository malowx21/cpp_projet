cmake_minimum_required(VERSION 3.10)
project(ParticleSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)





# Sources

set(SOURCES
    src/main.cpp
    src/Vecteur.cpp
    src/Univers.cpp
)


# Headers 

set(HEADERS
    include/Vecteur.hpp
    include/Univers.hpp
    include/Particule.hpp
)




add_executable(simulation ${SOURCES} ${HEADERS})

#ou trouver les repertoires d'include
target_include_directories(simulation PRIVATE include)



