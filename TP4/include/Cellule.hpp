#ifndef CELLULE_HPP
#define CELLULE_HPP

#include "Vecteur.hpp"
#include "Particule.hpp"
#include <vector>
#include <array>

class Cellule {
private:
    std::vector<int> particules_ids;
    Vecteur centre;
    std::array<int, 27> voisins;
    int nb_voisins;
    int index;

public:
    Cellule() : centre(0,0,0), nb_voisins(0), index(-1) {
        voisins.fill(-1);
    }

    // Getters
    const std::vector<int>&    getParticules() const { 
        return particules_ids; 
    }
    const Vecteur& getCentre() const { 
        return centre;
    }
    const std::array<int, 27>& getVoisins()  const {
        return voisins;
    }

    int getNbVoisins() const { return nb_voisins; }
    int getIndex()     const { return index;      }

    // Setters
    void setCentre(const Vecteur& c)  { centre = c;}
    void setIndex(int i) { index = i;}
    void setNbVoisins(int n) { nb_voisins = n;}
    void setVoisin(int pos, int val) { 
        voisins[pos] = val;
    }

    // Gestion des particules
    void addParticule(int id)  { 
        particules_ids.push_back(id);
    }
    void vider() { 
        particules_ids.clear();
    }
    bool est_vide() const { return particules_ids.empty(); }
};

#endif
