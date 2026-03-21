cmake_minimum_required (VERSION 3.16.3)
project(Demo CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -pg)
add_link_options(-pg)
# Commentaire
add_executable(exe Particule.cpp)
target_link_libraries(exe sfml-graphics sfml-window sfml-system)