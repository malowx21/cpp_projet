#ifndef PARTICULE_HPP
#define PARTICULE_HPP
#include "Particule.hpp"
#include "Vecteur.hpp"
class Particule
{
public:
    Vecteur position;
    Vecteur vitesse;
    Vecteur force;

    double m;
    int id;

    void AffichePosition() const;
};
#endif