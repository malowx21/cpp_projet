#ifndef UNIVERS_HPP
#define UNIVERS_HPP
#include <vector>
#include "Particule.hpp"
#include "Vecteur.hpp"
class Univers
{
public:
    std::vector<Particule> particules;
    int dimension;
    double t;

    Univers(int dim);

    void calcul_forces();
    void avancer(double dt, double t_end);
    void afficher() const;
};
#endif