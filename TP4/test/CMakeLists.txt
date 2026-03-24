# Créer un executable à partir de fichiers sources
add_executable(test_particule  test_particule.cxx)
add_executable(test_force  test_force.cxx)
add_executable(test_univers  test_univers.cxx)
add_executable(test_vecteur  test_vecteur.cxx)

# Fait le lien avec la bibliothèque créée précedemment
# Les entêtes sont automatiquement détectés.
target_link_libraries(test_particule PRIVATE tp4_lib)
target_link_libraries(test_force PRIVATE tp4_lib)
target_link_libraries(test_univers PRIVATE tp4_lib)
target_link_libraries(test_vecteur PRIVATE tp4_lib)