#include "Particule.hpp"
#include <iostream>

//Constructeur
Particule::Particule() : position(), vitesse(), force(), m(0.0), id(0) {}


// Affichage de l'état de la particule 
void Particule::AffichePosition() const {
    std::cout << id << " : " << position.getX() << ", "
    << position.getY() << ", " << position.getZ() << "\n";
}
