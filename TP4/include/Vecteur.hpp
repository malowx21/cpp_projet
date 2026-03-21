#ifndef VECTEUR_HPP
#define VECTEUR_HPP
#include "Vecteur.hpp"

class Vecteur
{
public:
    double x, y, z;

    // Constructeurs
    Vecteur();
    Vecteur(double x_, double y_, double z_);

    // Norme
    double norm() const ;
    Vecteur operator+(const Vecteur& v) const ;
    Vecteur operator-(const Vecteur& v) const;
    Vecteur operator*(double a) const ;
    Vecteur operator/(double a) const ;
    //+=
    Vecteur& operator+=(const Vecteur& v) ;
};

#endif