#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include "vecteur.hpp"
#include "particule.hpp"
#include "universTp4.hpp"

int main() {
    // Paramètres du TP
    double L1 = 250.0;
    double epsilon = 5.0;
    double sigma = 1.0;
    double m = 1.0;
    double r_cut = 2.5 * sigma;
    double dt = 0.00005; // Pas de temps réel du TP
    double t_final = 19.5;
    
    // Distance d'équilibre r = 2^(1/6) * sigma
    double dist_inter = pow(2.0, 1.0/6.0) * sigma;

    std::vector<Particule> ma_collection;

    // --- Remplissage de la collection ---
    // Rectangle bleu (160 x 40)
    for (int i = 0; i < 160; ++i) {
        for (int j = 0; j < 40; ++j) {
            Vecteur pos(i * dist_inter + 40.0, j * dist_inter + 20.0, 0.0);
            Vecteur vit(0.0, 0.0, 0.0);
            Vecteur forc(0.0, 0.0, 0.0);
            ma_collection.push_back(Particule(pos, vit, forc, m, (int)ma_collection.size(), "bleu"));
        }
    }

    // Carré rouge (40 x 40)
    for (int i = 0; i < 40; ++i) {
        for (int j = 0; j < 40; ++j) {
            Vecteur pos(i * dist_inter + 100.0, j * dist_inter + 150.0, 0.0);
            Vecteur vit(0.0, -10.0, 0.0); // Vitesse vers le bas
            Vecteur forc(0.0, 0.0, 0.0);
            ma_collection.push_back(Particule(pos, vit, forc, m, (int)ma_collection.size(), "rouge"));
        }
    }

    // Initialisation de l'univers avec ta classe
    UniversTp4 univers(2, L1, r_cut, sigma, epsilon, ma_collection);

    std::ofstream file("simulation_data.csv");
    file << "t,x,y,couleur\n";

    double t = 0;
    int step = 0;
    // On enregistre tous les X pas pour que le fichier ne soit pas gigantesque
    // mais assez souvent pour voir le mouvement (ex: toutes les 0.1 unités de temps)
    int save_freq = 500; 

    std::cout << "Simulation lancee..." << std::endl;

    while (t < t_final) {
        univers.maj_cellules();
        univers.avancer(dt);

        if (step % save_freq == 0) {
            // CRUCIAL : On boucle sur la collection interne de l'univers 
            // car c'est elle qui est mise à jour par univers.avancer()
            for (const auto& p : ma_collection) {
                // Utilisation de tes getters getPosition() et getCat()
                file << t << "," << p.getPosition()[0] << "," << p.getPosition()[1] << "," << p.getCat() << "\n";
            }
        }
        
        t += dt;
        step++;
    }

    file.close();
    std::cout << "Termine. Fichier simulation_data.csv pret." << std::endl;
    return 0;
}