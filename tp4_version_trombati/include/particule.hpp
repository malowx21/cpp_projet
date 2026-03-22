# pragma once
#include <iostream>
#include <vecteur.hpp>


class Particule {
    Vecteur position; 
    Vecteur vitesse; 
    Vecteur force; 
    double masse;
    int ident;
    std::string catg;
    public :
        Particule(Vecteur& pos, Vecteur& vit, Vecteur& forc,double masse, int ident , std::string cat); 
        Vecteur& getPosition();
        Vecteur& getVitesse();
        Vecteur& getForce();
        const Vecteur& getPosition() const;
        const Vecteur& getVitesse() const;
        const Vecteur& getForce() const;
        double getMasse() const;
        int getIdent() const;
        const std::string& getCat() const;
        void setIdent(int x);
        void setCat(std::string x);
        void setMasse(double m);
        double& operator[](int i);

};


// const Vecteur v = p.getPosition();

