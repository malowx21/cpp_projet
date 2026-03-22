#pragma once 
#include <iostream>



class Vecteur {
    double x, y, z;
    public :
        Vecteur(double x, double y , double z);
        void getVecteur();
        double& operator[](int i);
        const double& operator[](int i) const;
        Vecteur operator+(const Vecteur& v2) const;
        Vecteur operator-(const Vecteur& v2) const;
        Vecteur operator*(const Vecteur& v2) const;
        Vecteur& operator+=(const Vecteur& v2);
        Vecteur& operator-=(const Vecteur& v2);
        
};

