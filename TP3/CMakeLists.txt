cmake_minimum_required(VERSION 3.20)
project(ParticleSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# sources communes aux deux executables (sans main.cpp)
set(SOURCES_LIB
    src/Vecteur.cpp
    src/Univers.cpp
    src/Particule.cpp
)

set(HEADERS
    include/Vecteur.hpp
    include/Univers.hpp
    include/Particule.hpp
    include/export_vtk.hpp
)

# executable principal
add_executable(simulation src/main.cpp ${SOURCES_LIB} ${HEADERS})
target_include_directories(simulation PRIVATE include)

# chargement de googletest depuis github
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

# executable de tests
enable_testing()
add_executable(tests test/tests.cpp ${SOURCES_LIB})
target_include_directories(tests PRIVATE include)
target_link_libraries(tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
