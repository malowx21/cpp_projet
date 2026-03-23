#ifndef UNIVERS_HPP
#define UNIVERS_HPP
#include <vector>
#include "Particule.hpp"
#include "Vecteur.hpp"

class Univers
{
private:
    std::vector<Particule> particules;
    int dimension;
    double t;

public:
    Univers(int dim);

    // Getters
    const std::vector<Particule>& getParticules() const { return particules; }
    std::vector<Particule>&       getParticules()       { return particules; }
    int    getDimension() const { return dimension; }
    double getTemps()     const { return t;         }

    void ajouterParticule(const Particule& p);
    void calcul_forces();
    void avancer(double dt, double t_end);
    void afficher() const;
};

#endif
