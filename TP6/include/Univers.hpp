/**
 * @file Univers.hpp
 * @brief Définition de la classe Univers représentant le domaine de simulation.
 */

#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include "Vecteur.hpp"
#include "Particule.hpp"
#include "Cellule.hpp"
#include <vector>
#include <array>
#include <cmath>

/**
 * @class Univers
 * @brief Représente le domaine de simulation des particules.
 *
 * L'univers contient l'ensemble des particules et gère leur évolution
 * temporelle. Il est défini par sa dimension (1, 2 ou 3), ses longueurs caractéristiques @c L,
 * un rayon de coupure @c rcut et les paramètres physiques du potentiel
 * de Lennard-Jones (@c epsilon, @c sigma).
 *
 * L'espace est découpé en une grille de cellules .
 * Seules les particules situées dans des cellules voisines (3 en 1D,9 en 2D, 27 en 3D)
 *  et à une distance inférieure à @c rcut interagissent.
 *
 *
 * @note Le maillage doit être initialisé via initialiser_grille() avant
 *       d'appeler avancer().
 *
 */
class Univers {
private:
    int dimension;
    int type_border;
    std::vector<Particule> particules;  
    double t;


    // Paramètres physiques
    double epsilon;
    double sigma;
    double rcut;                        
    std::vector<double> L;

    // Paramètres du maillage
    std::array<int, 3> n_cubes;         
    std::vector<Cellule> cellules;       
    std::array<double, 3> taille_cellule;



public:
    /**
     * @brief Constructeur de l'univers de simulation.
     * @param dim Dimension 
     * @param eps Paramètre epsilon du potentiel de Lennard-Jones 
     * @param sig Paramètre sigma du potentiel de Lennard-Jones 
     * @param r_cut Rayon de coupure des interactions 
     * @param longueurs Dimensions du domaine de simulation 
     */
    Univers(int dim, double eps = 1.0, double sig = 1.0, double r_cut = 2.5,
            std::vector<double> longueurs = {250.0, 40.0, 40.0});

    /**
     * @brief Retourne la liste des particules modifiable.
     * @return Référence vers le vecteur de particules.
     */
    std::vector<Particule>& get_particules() { 
        return particules;
    }
    int get_type_boder() const{
        return type_border;
    }
    /**
     * @brief Retourne la liste des particules lecture seule.
     * @return Référence constante vers le vecteur de particules.
     */
    const std::vector<Particule>& get_particules() const { 
        return particules;
    }

    /**
     * @brief Retourne le temps courant de la simulation.
     * @return Temps t.
     */
    double get_temps() const {
        return t;
    }

    /**
     * @brief Retourne la dimension de la simulation.
     * @return Dimension .
     */
    int get_dimension() const { 
        return dimension;
    }

  
    /**
     * @brief Ajoute une particule dans l'univers et la place dans la cellule correspondante.
     * @param p La particule à ajouter.
     */
    void ajouter_particule(const Particule& p);

    /**
     * @brief Initialise la grille de cellules du maillage spatial.
     *
     * Calcule le nombre de cellules par dimension,crée les cellules, 
     * calcule leur centre, détermine les voisinages (jusqu'à 27 voisins en 3D)
     *  et affecte chaque particule à sa cellule en fonction de sa position courante.
     *
     */
    void initialiser_grille();

    /**
     * @brief Calcule les forces de Lennard-Jones entre particules voisines.
     *
     * Pour chaque particule, seules les particules dans les cellules voisines
     * sont examinées. 
     * La symétrie des forces est exploitée : @f$ F_{ji} = -F_{ij} @f$.
     */
    void calculer_forces_lj();

    /**
     * @brief Met à jour l'appartenance des particules aux cellules.
     *
     * Vide toutes les cellules puis réaffecte chaque particule à la cellule
     * correspondant à sa position courante. À appeler après chaque déplacement.
     */
    void mettre_a_jour_cellules();

    /**
     * @brief Retourne l'indice de la cellule contenant une position donnée.
     * @param pos Le vecteur position à localiser.
     * @return L'indice de la cellule dans le tableau @c cellules.
     */
    int get_cellule_index(const Vecteur& pos) const;

    /**
     * @brief Retourne les indices des cellules voisines d'une cellule.
     * @param cellule_idx L'indice de la cellule dont on cherche les voisines.
     * @return Vecteur des indices des cellules voisines valides.
     */
    std::vector<int> get_voisins(int cellule_idx) const;

    /**
     * @brief Fait avancer la simulation jusqu'à un temps final.
     *
     * Intègre les équations du mouvement par pas de temps @p dt en utilisant
     * l'algorithme de Störmer-Verlet. Les forces de Lennard-Jones sont recalculées
     * à chaque pas et la gravité peut être activée optionnellement.
     *
     * @param dt Pas de temps de l'intégration.
     * @param t_end Temps final de la simulation.
     * @param utiliser_gravite Active la gravité si true par défaut = false.
     */
    void avancer(double dt, double t_end, bool utiliser_gravite = false, double g = 12.0);

    /**
     * @brief Affiche des statistiques sur l'occupation de la grille.
     *
     * Indique le nombre de cellules vides, le nombre moyen de particules
     * par cellule et la cellule la plus peuplée.
     */
    void afficher_stats_grille() const;
    void appliquer_conditions_limites();
    void ajouter_forces_parois();
    void ajouter_gravite(double g);
    double energie_cinetique() const;
    void rescaler_vitesses(double Ec_cible);
};
#endif