#include "Particule.hpp"
#include <iostream>

//Constructeurs 

//Constructeur par defaut : crée particule vide 
Particule::Particule() : position(), vitesse(), force(), m(0.0), id(0) {}

// Constructeur par copie : crée une particule avec les 
// memes attributs que la particule  passée en paramètre . 
Particule::Particule(const Particule& autre): position(autre.position), vitesse(autre.vitesse),
                                              force(autre.force), m(autre.m), id(autre.id) {}

// Surcharge de l'opérateur d'affectation : recopie les attributs
// de la particule passée en paramètre .                                           
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

// Surcharge de l'opérateur de flux de sortie afin d'afficher 
// l'état de la particule : position, vitesse, force, masse, id.
std::ostream& operator<<(std::ostream& os, const Particule& p) {
    os << "Particule " << p.id << " , position : " 
    << p.position << " , vitesse  : " << p.vitesse 
    << ", force : " << p.force << " , masse    : " << p.m;
    return os;
}

//Destructeur 
Particule::~Particule(){}