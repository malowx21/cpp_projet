cmake_minimum_required(VERSION 3.10)

project(TP4)

set(CMAKE_CXX_STANDARD 17)

# sources du projet
file(GLOB SOURCES "src/*.cxx")

# bibliothèque du projet
add_library(tp4_lib ${SOURCES})

target_include_directories(tp4_lib PUBLIC include)

# sous-dossiers
add_subdirectory(demo)
add_subdirectory(test)