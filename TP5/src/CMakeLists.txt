## Indique les fichiers sources de la bibliothèque
add_library(tp5_lib Particule.cxx Univers.cxx Vecteur.cxx)

## Lie les entêtes à la bibliothèque
target_include_directories(tp5_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)