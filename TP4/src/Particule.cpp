#include "Particule.hpp"
#include <iostream>

//Constructeur par defaut
Particule::Particule() : position(), vitesse(), force(), m(0.0), id(0) {}

void Particule::AffichePosition() const {
    std::cout << id << " : "
              << position.getX() << ", "
              << position.getY() << ", "
              << position.getZ() << "\n";
}
