#ifndef PARTICULE_HPP
#define PARTICULE_HPP
#include "Vecteur.hpp"

class Particule
{
private:
    Vecteur position;
    Vecteur vitesse;
    Vecteur force;
    double m;
    int id;

public:
    // Constructeur
    Particule();

    // Getters constants pour la lecture 
    const Vecteur& getPosition() const { 
        return position;
    }
    const Vecteur& getVitesse() const { 
        return vitesse;  
    }
    const Vecteur& getForce() const { 
        return force;    
    }
    double getMasse() const { 
        return m;
    }
    int getId() const { 
        return id;
    }

    // Getters non-constants pour effectuer des modifications
    Vecteur& getPosition() { 
        return position;
    }
    Vecteur& getVitesse(){ 
        return vitesse;
    }
    Vecteur& getForce(){ 
        return force;
    }

    // Setters
    void setPosition(const Vecteur& p) { position = p;}
    void setVitesse(const Vecteur& v)  { vitesse  = v;}
    void setForce(const Vecteur& f) { force    = f;}
    void setMasse(double m_) { m  = m_;}
    void setId(int id_){ id = id_;     }

    void AffichePosition() const;
};

#endif
