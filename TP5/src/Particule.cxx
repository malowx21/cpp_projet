#include "Particule.hpp"
#include <iostream>

//Constructeurs
Particule::Particule() : position(), vitesse(), force(), m(0.0), id(0) {}

Particule::Particule(const Particule& autre): position(autre.position), vitesse(autre.vitesse),
                                              force(autre.force), m(autre.m), id(autre.id) {}

Particule& Particule::operator=(const Particule& autre){
    if (this!= &autre ){
        position = autre.position;
        vitesse = autre.vitesse;
        force = autre.force;
        m = autre.m;
        id = autre.id; 
    }
    return *this;
}

// Affichage de l'état de la particule 
void Particule::AffichePosition() const {
    std::cout << id << " : " << position.getX() << ", "
    << position.getY() << ", " << position.getZ() << "\n";
}

//Destructeur 

Particule::~Particule(){}