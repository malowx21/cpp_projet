#include <iostream>
#include <vecteur.hpp>
#include <stdexcept>


Vecteur::Vecteur(double x1, double y1,double z1) : x(x1), y(y1), z(z1){}

void Vecteur::getVecteur(){std::cout << "("<<x <<", "<<y<<", "<<z<<")"<<std::endl;}

double& Vecteur::operator[](int i ){
    if (i < 0 || i > 2) throw std::out_of_range("Index out of bounds");
    if (i==0) return x;
    if (i==1) return y;
    return z;
}

const double& Vecteur::operator[](int i) const {
    if (i < 0 || i > 2) throw std::out_of_range("Index out of bounds");
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
}

Vecteur Vecteur::operator+(const Vecteur& v2) const{
    return Vecteur (x+v2[0],y+v2[1],z+v2[2]);
}

Vecteur Vecteur::operator-(const Vecteur& v2) const{
    return Vecteur(x-v2[0],y-v2[1],z-v2[2]);

}
Vecteur Vecteur::operator*(const Vecteur& v2) const{
    return Vecteur(x*v2[0],y*v2[1],z*v2[2]);  
}


Vecteur& Vecteur::operator+=(const Vecteur& v2 ) {
    x+= v2.x;
    y+= v2.y;
    z+= v2.z;
    return *this;

}

Vecteur& Vecteur::operator-=(const Vecteur& v2 ) {
    x-= v2.x;
    y-= v2.y;
    z-= v2.z;
    return *this;
}




