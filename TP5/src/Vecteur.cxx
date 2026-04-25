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

Vecteur Vecteur::operator*(double a) const {
    return Vecteur(a*x, a*y, a*z);
}

Vecteur Vecteur::operator/(double a) const {
    if (std::abs(a) < 1e-12)
        throw std::runtime_error("Division by zero");
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

std::ostream& operator<<(std::ostream& os, const Vecteur& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

// Destructeur
Vecteur::~Vecteur(){}


Vecteur operator+(const Vecteur& a , const Vecteur& b){
    Vecteur c(a);
    c+=b;
    return c;
}

Vecteur operator-(const Vecteur& a , const Vecteur& b){
    Vecteur c(a);
    c-=b;
    return c;
}

