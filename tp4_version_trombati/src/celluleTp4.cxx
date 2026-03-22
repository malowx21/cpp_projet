#include <iostream>
#include <vector>
#include <celluleTp4.hpp>
#include <particule.hpp>


Cellule::Cellule(){}

std::vector<std::reference_wrapper<Particule>>& Cellule::get_particule() {
    return particules_cell;
}

std::vector<Cellule*>& Cellule::get_cellule_voisines()  {
    return cellules_voisines;
}


void Cellule::addParticule( Particule& p) {
    particules_cell.push_back(p);
}

void Cellule::addVoisin(Cellule* c) {
    cellules_voisines.push_back(c);
}

void Cellule::clearParticules(){
    particules_cell.clear();
}