/**
 * @file Univers.hpp
 * @brief Définition de la classe Univers représentant le domaine de simulation.
 */

#ifndef UNIVERS_HPP
#define UNIVERS_HPP
#include <vector>
#include "Particule.hpp"
#include "Vecteur.hpp"

/**
 * @class Univers
 * @brief Représente un univers de simulation contenant un ensemble de particules.
 *
 * L'univers est défini par sa dimension et contient une
 * collection de particules. Il gère leur évolution temporelle via
 * l'intégration des équations du mouvement sous interaction gravitationnelle.
 */
class Univers
{
private:
    std::vector<Particule> particules;
    int dimension;
    double t;

public:
    /**
     * @brief Constructeur de l'univers.
     * @param dim Dimension de la simulation 
     */
    Univers(int dim);

    /**
     * @brief Retourne la liste des particules lecture seule.
     * @return Référence constante vers le vecteur de particules.
     */
    const std::vector<Particule>& getParticules() const { 
        return particules;
    }

    /**
     * @brief Retourne la liste des particules (modifiable).
     * @return Référence vers le vecteur de particules.
     */
    std::vector<Particule>& getParticules() { 
        return particules;
    }

    /**
     * @brief Retourne la dimension de la simulation.
     * @return Dimension 
     */
    int getDimension() const { 
        return dimension;
    }

    /**
     * @brief Retourne le temps courant de la simulation.
     * @return Temps t.
     */
    double getTemps() const { 
        return t;
    }

    /**
     * @brief Ajoute une particule dans l'univers.
     * @param p La particule à ajouter.
     */
    void ajouterParticule(const Particule& p);

    /**
     * @brief Calcule les forces d'interaction gravitationnelle entre toutes les particules.
     */
    void calcul_forces();

    /**
     * @brief Fait avancer la simulation .
     *
     * @param dt    Pas de temps.
     * @param t_end Temps final de la simulation.
     */
    void avancer(double dt, double t_end);

    /**
     * @brief Affiche l'état courant de l'univers.
     */
    void afficher() const;
};

#endif