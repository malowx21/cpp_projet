// Cellule.hpp
#ifndef CELLULE_HPP
#define CELLULE_HPP

#include "Vecteur.hpp"
#include "Particule.hpp"
#include <vector>
#include <array>

class Cellule {
public:
    std::vector<int>    particules_ids;
    Vecteur             centre;
    std::array<int, 27> voisins;
    int                 nb_voisins;   // nombre de voisins valides dans voisins[]
    int                 index;

    Cellule() : centre(0,0,0), nb_voisins(0), index(-1) {
        voisins.fill(-1);
    }

    bool est_vide() const { return particules_ids.empty(); }
    void vider()          { particules_ids.clear(); }
};

#endif
