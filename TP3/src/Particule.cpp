#include "Particule.hpp"
#include <iostream>


#include "Vecteur.hpp"
 

void Particule::AffichePosition() const {
    std::cout << id << " : "
            << position.x << ", "
            << position.y << ", "
            << position.z << "\n";
}
