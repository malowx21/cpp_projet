#include "Vecteur.hpp"
#include "Particule.hpp"
#include "Univers.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>

//Q1 - Export du potentiel LJ pour tracé Python
void exporter_potentiel_lj(double eps, double sigma, const std::string& fichier) {
    std::ofstream f(fichier);
    f << "r,U\n";
    for (int i = 1; i <= 500; ++i) {
        double r   = 0.85 + i * (4.0 - 0.85) / 500.0;
        double sr6 = std::pow(sigma / r, 6);
        double U   = 4.0 * eps * (sr6 * sr6 - sr6);
        f << r << "," << U << "\n";
    }
    std::cout << "Potentiel LJ exporte -> " << fichier << "\n";
}

// Parametres simulation (Q6)
const double EPSILON = 5.0;
const double SIGMA   = 1.0;
const double RCUT    = 2.5 * SIGMA;
const double DT      = 0.00005;
const double T_END   = 19.5;
// Distance d'equilibre LJ : r* = 2^(1/6) * sigma
const double EQ_DIST = std::pow(2.0, 1.0 / 6.0) * SIGMA;

// Creation d'un rectangle de particules
void creer_rectangle(Univers& U,double x_min, double y_min, int nx, int ny,
        const Vecteur& v0, int& id_counter) {
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            Particule p;
            p.setPosition(Vecteur(x_min + i * EQ_DIST,
                            y_min + j * EQ_DIST,
                            0.0));
            p.setVitesse(v0);
            p.setForce(Vecteur(0, 0, 0));
            p.setMasse(1.0);
            p.setId(id_counter++);
            U.ajouter_particule(p);
}
}
}

//Ecriture d'une frame dans frames.csv
void ecrire_frame(std::ofstream& f, const Univers& U, double t) {
    f << "# t=" << t << "\n";
    for (const auto& p : U.get_particules()) {
        double speed = p.getVitesse().norm();
        f << p.getPosition().getX() << ","
          << p.getPosition().getY() << ","
          << speed                  << "\n";
    }
}

int main() {

    // Q1 : potentiel LJ
    exporter_potentiel_lj(EPSILON, SIGMA, "potentiel_lj.csv");

    // Q2/Q3 : univers 2D avec grille de cellules
    // L = (250, 200) pour contenir les deux blocs + marges
    Univers U(2, EPSILON, SIGMA, RCUT, {250.0, 200.0, 1.0});

    // Q6 : deux blocs de particules
    int id = 0;

    // bloc rouge : 40x40 particules, vitesse (0, -10) vers le bas
    int nx_r = 40, ny_r = 40;
    double w_bleu  = (160 - 1) * EQ_DIST;
    double w_rouge = (nx_r - 1) * EQ_DIST;
    double x_rouge = (w_bleu - w_rouge) / 2.0;
    double y_rouge = (40 + 5) * EQ_DIST;

    creer_rectangle(U, x_rouge, y_rouge, nx_r, ny_r, Vecteur(0, -10, 0), id);

    // bloc bleu : 160x40 particules, immobile
    creer_rectangle(U, 0.0, 0.0, 160, 40, Vecteur(0, 0, 0), id);

    std::cout << "Particules totales : " << id << "\n";
    U.afficher_stats_grille();

    // Boucle de simulation
    // on sauvegarde 1 frame toutes les SAVE_EVERY iterations
    // pour garder un fichier de taille raisonnable
    const int SAVE_EVERY = 200;

    std::ofstream f_frames("frames.csv");
    f_frames << "# N=" << id << " dt=" << DT << " rcut=" << RCUT << "\n";

    // frame initiale (t=0)
    ecrire_frame(f_frames, U, 0.0);

    double t = 0.0;
    int step = 0, frame_count = 1;

    auto t0 = std::chrono::high_resolution_clock::now();
    std::cout << "Simulation en cours (ecriture toutes les "
              << SAVE_EVERY << " iterations)...\n";

    while (t < T_END) {
        //Q4 - forces LJ avec rayon de coupure + grille de cellules
        //Q5 - mettre_a_jour_cellules() est appele dans avancer()
        U.avancer(DT, T_END, false);
        t += DT;
        ++step;

        if (step % SAVE_EVERY == 0) {
            ecrire_frame(f_frames, U, t);
            ++frame_count;

            double pct     = 100.0 * t / T_END;
            double elapsed = std::chrono::duration<double>(
                             std::chrono::high_resolution_clock::now() - t0).count();
            std::cout << "\r  " << pct << "%  frames=" << frame_count
                      << "  t=" << elapsed << "s    " << std::flush;
        }
    }

    f_frames.close();

    double total = std::chrono::duration<double>(
                   std::chrono::high_resolution_clock::now() - t0).count();
    std::cout << "\nTermine en " << total << " s  |  "
              << frame_count << " frames -> frames.csv\n";
    std::cout << "Visualisation : python3 visualiser.py\n";

    return 0;
}