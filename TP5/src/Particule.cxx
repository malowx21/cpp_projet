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


std::ostream& operator<<(std::ostream& os, const Particule& p) {
    os << "Particule " << p.id << " , position : " 
    << p.position << " , vitesse  : " << p.vitesse 
    << ", force : " << p.force << " , masse    : " << p.m;
    return os;
}

//Destructeur 

Particule::~Particule(){}