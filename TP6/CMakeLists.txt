cmake_minimum_required(VERSION 3.10)

project(TP4)

set(CMAKE_CXX_STANDARD 17)

# sources du projet
file(GLOB SOURCES "src/*.cxx")

# bibliothèque du projet
add_library(tp4_lib ${SOURCES})

target_include_directories(tp4_lib PUBLIC include)
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()
# sous-dossiers
add_subdirectory(demo)
add_subdirectory(test)