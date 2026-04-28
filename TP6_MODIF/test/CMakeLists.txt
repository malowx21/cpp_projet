add_executable(tests test_particule.cxx test_force.cxx test_univers.cxx test_vecteur.cxx)

target_link_libraries(tests PRIVATE tp6_lib GTest::gtest_main)

include(GoogleTest)
add_test(NAME all_tests COMMAND tests)