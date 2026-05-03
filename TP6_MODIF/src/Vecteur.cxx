#include "Vecteur.hpp"
#include <cmath>

//Constructeurs

//Constructeur qui initialise les composantes de Vecteur 
Vecteur::Vecteur(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

// Constructeur par copie : crée un vecteur avec les memes
//  composantes que celles du vecteur passé en paramètre
Vecteur::Vecteur(const Vecteur& autre): x(autre.x), y(autre.y), z(autre.z) {}

// Surchage de l'opérateur d'affectation : recopie les composantes du vecteur passé en paramètre
// tout en évitant l'auto-affectation .
Vecteur& Vecteur::operator=(const Vecteur& autre){
    if (this != &autre){
        x = autre.x;
        y = autre.y;
        z = autre.z;
    }
    return *this;
}


//Norme : calcule la norme 2 du vecteur 
double Vecteur::norm() const {
    return std::sqrt(x*x + y*y + z*z);
}

// Opérations arithmétiques 

// Multiplie le vecteur par un scalaire et retourne le vecteur résultant .
Vecteur Vecteur::operator*(double a) const {
    return Vecteur(a*x, a*y, a*z);
}

// Divise le vecteur par un scalaire et retourne le vecteur résultant .
// Ici on lève une exception si le diviseur tend vers ou égal à 0
Vecteur Vecteur::operator/(double a) const {
    if (std::abs(a) < 1e-12)
        throw std::runtime_error("Division by zero");
    return Vecteur(x/a, y/a, z/a);
}

// Modification sur place en ajoutant les composantes du vecteur passé
// en paramètre aux composantes 
Vecteur& Vecteur::operator+=(const Vecteur& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}


// Modification sur place en soustrayant les composantes du vecteur passé
// en paramètre aux composantes 
Vecteur& Vecteur::operator-=(const Vecteur& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

// Surcharge de l'opérateur de flux de sortie afin d'afficher les composantes du vecteur
std::ostream& operator<<(std::ostream& os, const Vecteur& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

// Destructeur
Vecteur::~Vecteur(){}

// Operateur externes 

// Retourne la somme de deux vecteurs , sans les modifier
Vecteur operator+(const Vecteur& a , const Vecteur& b){
    Vecteur c(a);
    c+=b;
    return c;
}

// Retourne la difference de deux vecteurs, sans les modifier 
Vecteur operator-(const Vecteur& a , const Vecteur& b){
    Vecteur c(a);
    c-=b;
    return c;
}

