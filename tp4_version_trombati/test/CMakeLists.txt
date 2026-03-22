# Créer un executable à partir de fichiers sources
add_executable(simulation simulation.cxx)

# Fait le lien avec la bibliothèque créée précedemment
# Les entêtes sont automatiquement détectés.
target_link_libraries(simulation  lab3_lib)