#include "Vecteur.hpp"
#include <cmath>

//Constructeurs
Vecteur::Vecteur(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}


Vecteur::Vecteur(const Vecteur& autre): x(autre.x), y(autre.y), z(autre.z) {}


Vecteur& Vecteur::operator=(const Vecteur& autre){
    if (this != &autre){
        x = autre.x;
        y = autre.y;
        z = autre.z;
    }
    return *this;
}


//Norme
double Vecteur::norm() const {
    return std::sqrt(x*x + y*y + z*z);
}

// Opérations arithmétiques 
Vecteur Vecteur::operator+(const Vecteur& v) const {
    return Vecteur(x + v.x, y + v.y, z + v.z);
}

Vecteur Vecteur::operator-(const Vecteur& v) const {
    return Vecteur(x - v.x, y - v.y, z - v.z);
}

Vecteur Vecteur::operator*(double a) const {
    return Vecteur(a*x, a*y, a*z);
}

Vecteur Vecteur::operator/(double a) const {
    return Vecteur(x/a, y/a, z/a);
}

//Opérateur += 
Vecteur& Vecteur::operator+=(const Vecteur& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

//Opérateur -=
Vecteur& Vecteur::operator-=(const Vecteur& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

// Destructeur
Vecteur::~Vecteur(){}
