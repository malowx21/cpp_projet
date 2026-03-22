# Créer une bibliothèque à partir d'un ensemble de fichiers de définitions (.cxx) sans main.
# Ces fichiers ne contiennent pas de fonction main
# Remplacer ... par les fichiers nécessaires 
add_library(lab3_lib vecteur.cxx particule.cxx celluleTp4.cxx universTp4.cxx)

target_include_directories(lab3_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
