# Créer un executable à partir de fichiers sources
add_executable(demo_univers demo_univers.cxx)

# Fait le lien avec la bibliothèque créée précedemment
# Les entêtes sont automatiquement détectés.
target_link_libraries(demo_univers PRIVATE tp4_lib)