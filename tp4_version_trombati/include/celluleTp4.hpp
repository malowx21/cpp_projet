#pragma once
#include <iostream>
#include <particule.hpp>
#include <vector>



class Cellule{

    std::vector<std::reference_wrapper<Particule>> particules_cell;
    std::vector<Cellule*> cellules_voisines;
    public :
        Cellule();
        std::vector<std::reference_wrapper<Particule>>& get_particule() ;
        // const std::vector<Particule>& get_particule() const ;
        std::vector<Cellule*>& get_cellule_voisines() ;
        void addParticule( Particule& p);
        void addVoisin(Cellule* c);
        void clearParticules();
};  