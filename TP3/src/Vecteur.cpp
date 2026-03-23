#include "Vecteur.hpp"
#include <cmath>

//Constructeurs
Vecteur::Vecteur() : x(0), y(0), z(0) {}
Vecteur::Vecteur(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

//Norme
double Vecteur::norm() const {
    return std::sqrt(x*x + y*y + z*z);
}

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

//+=
Vecteur& Vecteur::operator+=(const Vecteur& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

//-=
Vecteur& Vecteur::operator-=(const Vecteur& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}
