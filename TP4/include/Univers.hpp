// Univers.hpp (complété)
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
    double t;                           // Temps courant
    
    //paramètres physiques
    double epsilon;                      //profondeur du potentiel LJ
    double sigma;                        
    double rcut;                         
    std::vector<double> L;      
    
    //paramètres du maillage
    std::array<int, 3> n_cubes;          // Nombre de cubes dans chaque direction
    std::vector<Cellule> cellules;       // Grille de cellules
    std::array<double, 3> taille_cellule; // Taille d'une cellule
    
public:
    // Constructeur
    Univers(int dim, 
            double eps = 1.0, 
            double sig = 1.0, 
            double r_cut = 2.5,
            std::vector<double> longueurs = {250.0, 40.0, 40.0});
    
    //getters/setters
    std::vector<Particule>& get_particules() { return particules; }
    const std::vector<Particule>& get_particules() const { return particules; }
    double get_temps() const { return t; }
    
    
    void ajouter_particule(const Particule& p);
    
   
    void initialiser_grille();
    int get_dimension() const { return dimension; }
    // Calcul des forces (Lennard-Jones)
    void calculer_forces_lj();
    
    // Mise à jour des cellules après déplacement
    void mettre_a_jour_cellules();
    
    // Obtenir l'index de la cellule pour une position
    int get_cellule_index(const Vecteur& pos) const;
    
    // Voisins d'une cellule
    std::vector<int> get_voisins(int cellule_idx) const;
    
    // Intégration
    void avancer(double dt, double t_end, bool utiliser_gravite = false);
    
    // Debug
    void afficher_stats_grille() const;
};

#endif