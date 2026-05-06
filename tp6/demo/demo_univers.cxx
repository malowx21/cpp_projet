#include "Vecteur.hpp"
#include "Particule.hpp"
#include "Univers.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <filesystem>

// Paramètres d'initialisation de l'univers comme dans le tp

// Paramètres du potentiel 
const double EPSILON = 1.0;
const double SIGMA = 1.0;
const double RCUT = 2.5 * SIGMA;

// Pas de temps et durée totale de simulation
const double DT = 0.0005;
const double T_END = 29.5;

// Nombre cible de particules
const int N1 = 395;
const int N2 = 17227; 

// Gravité
const double G = -12.0;
// Energie cinetique cible 
const double ECIBLE = 0.005 * (N1 + N2);

// L'export VTK : création des frames dans un dossier vtk_output 
// pour pouvoir visualiser la simulation en utilisant Paraview .
void write_vtk(const Univers& U, int frame, int N_sea) {

    std::string filename = "vtk_output/frame_" + std::to_string(frame) + ".vtk";
    std::ofstream f(filename);

    const auto& P = U.get_particules();
    int N = P.size();

    f << "# vtk DataFile Version 3.0\n";
    f << "Particles\nASCII\nDATASET POLYDATA\n";
    f << "POINTS " << N << " double\n";

    for (const auto& p : P)
        f << p.getPosition().getX() << " "
          << p.getPosition().getY() << " "
          << p.getPosition().getZ() << "\n";

    f << "VERTICES " << N << " " << 2*N << "\n";
    for (int i = 0; i < N; i++)
        f << "1 " << i << "\n";

    // Champ des scalaires  : on notera  0 pour la  mer et  1 pour le disque
    f << "POINT_DATA " << N << "\n";
    f << "SCALARS group int 1\n";
    f << "LOOKUP_TABLE default\n";
    for (int i = 0; i < N; i++)
        f << (i < N_sea ? 0 : 1) << "\n";

    f.close();
}

// Création du disque (boule descendante dans la simulation )
void creer_disque(Univers& U, double cx, double cy,
                 double rayon, double espacement,
                 Vecteur v0, int& id) {

    // Parcours d'une grille  autour du centre                
    for (double x = cx - rayon; x <= cx + rayon; x += espacement) {
        for (double y = cy - rayon; y <= cy + rayon; y += espacement) {

            // Distance au centre 
            double dx = x - cx;
            double dy = y - cy;

            // Les points du disque
            if (dx*dx + dy*dy <= rayon*rayon) {
                Particule p;
                // Initialisatin 
                p.setPosition(Vecteur(x, y, 0));
                p.setVitesse(v0);
                p.setForce(Vecteur(0,0,0));
                p.setMasse(3.0);
                p.setId(id++);
                U.ajouter_particule(p);
            }
        }
    }
}

// Création de la mer (partie statique dans la simulation)
void creer_mer(Univers& U, double width, double height,
                double espacement, int& id) {

    for (double x = 2.0; x < width; x += espacement) {
        for (double y = 2.0; y < height; y += espacement) {

            Particule p;
            p.setPosition(Vecteur(x, y, 0));
            //double v0 = 0.5;
            // p.setVitesse(Vecteur(
            //     v0 * (rand() / double(RAND_MAX) - 0.5),
            //     v0 * (rand() / double(RAND_MAX) - 0.5),
            //     0));
            p.setVitesse(Vecteur(0,0,0));
            p.setForce(Vecteur(0,0,0));
            p.setMasse(1.0);
            p.setId(id++);
            U.ajouter_particule(p);
        }
    }
}

int main() {
    int ret = system("rm -rf vtk_output");
    (void)ret;
   
    std::filesystem::create_directories("vtk_output");

    // Paramètres exacts de l'énoncé : L1=250, L2=180
    Univers U(2, EPSILON, SIGMA, RCUT, {250.0, 180.0, 1.0}, 0);

    int id = 0;

    // double espacement = 1.008 * SIGMA; 
    // espacement pour obtenir un peu près N2=17227 sur 250×70
    double espacement = 1.00776 * SIGMA; 

    // Mer : N2 approximativement 17227 particules en bas
    creer_mer(U, 250.0, 70.0, espacement, id);
    int N_sea = id;
    // Disque : N1 approximativement 395 particules, vitesse v=(0,10) vers le bas
    creer_disque(U,125.0, 150.0,  11.0, espacement,Vecteur(0, -10, 0),id);

    std::cout << "Particles: " << id << "\n";

    int step = 0;
    int frame = 0;

    while (U.get_temps() < T_END) {

        U.avancer(DT, T_END, true, G);
        if (step % 100 == 0)   U.check_validite();

        // Rescaling toutes 1000 itérations comme indiqué dans le tp 
        if (step > 4000 && step % 1000 == 0)
            U.rescaler_vitesses(ECIBLE);

        if (step > 100 && step % 50 == 0)
            write_vtk(U, frame++,N_sea);

        step++;
    }

    std::cout << "Simulation Terminé.\n";
}