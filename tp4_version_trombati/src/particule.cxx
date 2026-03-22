#include <iostream>
#include <stdexcept>
#include <vecteur.hpp>
#include <particule.hpp>


Particule::Particule(Vecteur& pos, Vecteur& vit, Vecteur& forc,double masse, int ident, std::string categ) :
            position(pos), vitesse(vit), force(forc),masse(masse), ident(ident), catg(categ){}


Vecteur& Particule::getPosition(){return position;}
Vecteur& Particule::getVitesse(){return vitesse;}
Vecteur& Particule::getForce(){return force;}
const Vecteur& Particule::getPosition() const { return position;}
const Vecteur& Particule::getVitesse() const { return vitesse;}
const Vecteur& Particule::getForce() const{ return force;}


double Particule::getMasse()const { return masse;}
int Particule::getIdent() const { return ident;}
const std::string& Particule::getCat() const{ return catg;}


double& Particule::operator[](int i){
    if (i<0 || i>9)  throw std::out_of_range("Index out of bounds");
    if (i==0) return position[0];
    if (i==1) return position[1];
    if (i==2) return position[2];
    if (i==3) return vitesse[0];
    if (i==4) return vitesse[1];
    if (i==5) return vitesse[2];
    if (i==6) return force[0];
    if (i==7) return force[1];
    if (i==8) return force[2];
    return masse;
}

void Particule::setIdent(int x){
    ident = x;
}

void Particule::setCat(std::string x){
    catg= x;
}

void Particule::setMasse(double x){
    masse = x;
}
