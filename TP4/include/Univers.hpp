#ifndef UNIVERS_HPP
#define UNIVERS_HPP

#include "Vecteur.hpp"
#include "Particule.hpp"
#include "Cellule.hpp"
#include <vector>
#include <array>
#include <cmath>

class Univers {
private:
    int dimension;
    std::vector<Particule> particules;
    double t;
    
    //paramètres physiques
    double epsilon;
    double sigma;                        
    double rcut;                         
    std::vector<double> L;      
    
    //paramètres du maillage
    std::array<int, 3> n_cubes;          
    std::vector<Cellule> cellules;
    std::array<double, 3> taille_cellule;
    
public:
    // Constructeur
    Univers(int dim, double eps = 1.0, double sig = 1.0, double r_cut = 2.5,
            std::vector<double> longueurs = {250.0, 40.0, 40.0});
    
    // Getters
    std::vector<Particule>& get_particules() { 
        return particules;
    }
    const std::vector<Particule>& get_particules()const { 
        return particules;
    }
    double get_temps() const { 
        return t;
    }
    int get_dimension()const { 
        return dimension;
    }
    
    // Ajout d'une particule dans une cellule 
    void ajouter_particule(const Particule& p);
    
    // Initialisation de la grille des cellules
    void initialiser_grille();

    // Calcul des forces (Lennard-Jones)
    void calculer_forces_lj();
    
    // Mise à jour des cellules après déplacement
    void mettre_a_jour_cellules();
    
    // Obtenir l'index de la cellule pour une position
    int get_cellule_index(const Vecteur& pos) const;
    
    // Voisins d'une cellule
    std::vector<int> get_voisins(int cellule_idx) const;
    
    // Avancement de l'état de la grille dans le temps de dt 
    void avancer(double dt, double t_end, bool utiliser_gravite = false);
    
    void afficher_stats_grille() const;
};

#endif