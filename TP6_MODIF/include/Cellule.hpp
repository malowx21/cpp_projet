/**
 * @file Cellule.hpp
 * @brief Définition de la classe Cellule représentant une cellule du maillage spatial.
 */

#ifndef CELLULE_HPP
#define CELLULE_HPP

#include "Vecteur.hpp"
#include "Particule.hpp"
#include <vector>
#include <array>

/**
 * @class Cellule
 * @brief Représente une cellule élémentaire du maillage spatial de l'univers.
 *
 * L'univers est découpé en une grille de cellules pour optimiser
 * le calcul des forces : seules les particules dans des cellules voisines
 * interagissent. Chaque cellule connaît ses voisines (jusqu'à 27 en 3D),
 * son centre géométrique et la liste des identifiants des particules qu'elle contient.
 */
class Cellule {
private:
    std::vector<int> particules_ids; 
    Vecteur centre;
    std::array<int, 27> voisins;
    int nb_voisins;                 
    int index;

public:
    /**
     * @brief Constructeur par défaut.
     *
     * Initialise la cellule avec un centre à l'origine, aucun voisin et un index invalide -1.
     */
    Cellule() : centre(0,0,0), nb_voisins(0), index(-1) {
        voisins.fill(-1);
    }


    /**
     * @brief Constructeur par copie 
     */
    Cellule(const Cellule& autre) = default;

    /**
     * @brief Opérateur d'affectation par copie.
     * @param autre Cellule source .
     * @return référence vers cette cellule modifiée. 
     */
    Cellule& operator=(const Cellule& autre) = default;


    /**
     * @brief Retourne la liste des identifiants des particules dans la cellule.
     * @return Référence constante vers le vecteur d'identifiants.
     */
    const std::vector<int>& getParticules() const { 
        return particules_ids;
    }

    /**
     * @brief Retourne le centre géométrique de la cellule.
     * @return Référence constante vers le vecteur centre.
     */
    const Vecteur& getCentre() const { 
        return centre;
    }

    /**
     * @brief Retourne le tableau des indices des cellules voisines.
     * @return Référence constante vers le tableau de 27 voisins (valeur -1 si absent).
     */
    const std::array<int, 27>& getVoisins() const { 
        return voisins;
    }

    /**
     * @brief Retourne le nombre de voisins valides de la cellule.
     * @return Nombre de voisins.
     */
    int getNbVoisins() const { 
        return nb_voisins;
    }

    /**
     * @brief Retourne l'indice global de la cellule dans la grille.
     * @return Indice de la cellule.
     */
    int getIndex() const { 
        return index;
    }

    /**
     * @brief Modifie le centre géométrique de la cellule.
     * @param c Nouveau vecteur centre.
     */
    void setCentre(const Vecteur& c) { 
        centre = c;
    }

    /**
     * @brief Modifie l'indice global de la cellule.
     * @param i Nouvel indice.
     */
    void setIndex(int i) { 
        index = i;
    }

    /**
     * @brief Modifie le nombre de voisins valides.
     * @param n Nombre de voisins.
     */
    void setNbVoisins(int n) { 
        nb_voisins = n;
    }

    /**
     * @brief Définit l'indice d'un voisin à une position donnée.
     * @param pos Position dans le tableau des voisins (0 à 26).
     * @param val Indice de la cellule voisine (-1 si absente).
     */
    void setVoisin(int pos, int val) { 
        voisins[pos] = val;
    }

    /**
     * @brief Ajoute une particule dans la cellule.
     * @param id Identifiant de la particule à ajouter.
     */
    void addParticule(int id) { 
        particules_ids.push_back(id);
    }

    /**
     * @brief Supprime toutes les particules de la cellule.
     *
     * Utilisé lors de la mise à jour du maillage après déplacement des particules.
     */
    void vider() { 
        particules_ids.clear();
    }

    /**
     * @brief Indique si la cellule ne contient aucune particule.
     * @return true si la cellule est vide, false sinon.
     */
    bool est_vide() const { 
        return particules_ids.empty();
    }

    /**
     * @brief Destructeur 
     */
    ~Cellule() = default;
};

#endif