/**
 * @file Particule.hpp
 * @brief Définition de la classe Particule représentant une particule physique.
 */

#ifndef PARTICULE_HPP
#define PARTICULE_HPP
#include "Vecteur.hpp"

/**
 * @class Particule
 * @brief Représente une particule physique.
 *
 * Une particule est caractérisée par sa position, sa vitesse, la force
 * qui lui est appliquée, sa masse et un identifiant unique.
 * Les getters sont disponibles en version constante en lecture et
 * non-constante modification directe.
 */
class Particule
{
private:
    Vecteur position; 
    Vecteur vitesse;  
    Vecteur force;
    double m;
    int id;

public:
    /**
     * @brief Constructeur par défaut.
     *
     * Initialise la particule avec une position, vitesse et force nulles.
     */
    Particule();

    /**
     * @brief Constructeur de copie.
     * @param autre la particule à copier.
     */
    Particule(const Particule& autre);

    /**
     * @brief Opérateur d'affectation par copie.
     * @param autre la paticule source.
     * @return Référence vers cette particule modifiée.
     */
    Particule& operator=(const Particule& autre);

    /**
     * @brief Retourne la position de la particule lecture seule.
     * @return Référence constante vers le vecteur position.
     */
    const Vecteur& getPosition() const { 
        return position; 
    }

    /**
     * @brief Retourne la vitesse de la particule lecture seule.
     * @return Référence constante vers le vecteur vitesse.
     */
    const Vecteur& getVitesse() const { 
        return vitesse;
    }

    /**
     * @brief Retourne la force appliquée à la particule lecture seule.
     * @return Référence constante vers le vecteur force.
     */
    const Vecteur& getForce() const { 
        return force;
    }

    /**
     * @brief Retourne la masse de la particule.
     * @return La masse m.
     */
    double getMasse() const { 
        return m;
    }

    /**
     * @brief Retourne l'identifiant de la particule.
     * @return L'identifiant unique id.
     */
    int getId() const { 
        return id;
    }

    /**
     * @brief Retourne la position de la particule modifiable.
     * @return Référence non-constante vers le vecteur position.
     */
    Vecteur& getPosition() { 
        return position;
    }

    /**
     * @brief Retourne la vitesse de la particule modifiable.
     * @return Référence non-constante vers le vecteur vitesse.
     */
    Vecteur& getVitesse() { 
        return vitesse;
    }

    /**
     * @brief Retourne la force de la particule modifiable.
     * @return Référence non-constante vers le vecteur force.
     */
    Vecteur& getForce() { 
        return force;
    }

    /**
     * @brief Modifie la position de la particule.
     * @param p Nouveau vecteur position.
     */
    void setPosition(const Vecteur& p) { 
        position = p;
    }

    /**
     * @brief Modifie la vitesse de la particule.
     * @param v Nouveau vecteur vitesse.
     */
    void setVitesse(const Vecteur& v) { 
        vitesse = v;
    }

    /**
     * @brief Modifie la force appliquée à la particule.
     * @param f Nouveau vecteur force.
     */
    void setForce(const Vecteur& f) { 
        force = f;
    }

    /**
     * @brief Modifie la masse de la particule.
     * @param m_ Nouvelle valeur de masse.
     */
    void setMasse(double m_) { 
        m = m_;
    }

    /**
     * @brief Modifie l'identifiant de la particule.
     * @param id_ Nouvel identifiant.
     */
    void setId(int id_) { 
        id = id_;
    }

    /**
     * @brief Affiche la position de la particule sur la sortie standard.
     */
    void AffichePosition() const;

    /**
     * @brief Destructeur 
     */
    ~Particule() ;
};

#endif