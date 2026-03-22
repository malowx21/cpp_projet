#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <chrono>
#include "vecteur.hpp"
#include "particule.hpp"
#include "universTp4.hpp"

int main() {

   
    double sigma   = 1.0;
    double epsilon = 5.0;
    double m       = 1.0;
    double r_cut   = 2.5 * sigma;
    double dt      = 0.00005;
    double t_final = 19.5;

  
    double d = pow(2.0, 1.0 / 6.0) * sigma;   // ~1.122

    // Dimensions des deux blocs
    int nx_bleu = 160, ny_bleu = 40;
    int nx_rouge = 40, ny_rouge = 40;

    double w_bleu  = (nx_bleu  - 1) * d;
    double h_bleu  = (ny_bleu  - 1) * d;
    double w_rouge = (nx_rouge - 1) * d;
    double h_rouge = (ny_rouge - 1) * d;

    // Position des blocs
    double x0_bleu  = 5.0;
    double y0_bleu  = 5.0;
    double x0_rouge = x0_bleu + (w_bleu - w_rouge) / 2.0;  // centre
    double y0_rouge = y0_bleu + h_bleu + 10.0;              // au-dessus

    // Ld doit englober tout + marge pour la chute
    double Ld = std::max(x0_bleu + w_bleu + 5.0,
                         y0_rouge + h_rouge + 50.0);

    std::cout << "=== Parametres ===" << std::endl;
    std::cout << "  d=" << d << "  Ld=" << Ld
              << "  r_cut=" << r_cut << "  dt=" << dt
              << "  t_final=" << t_final << std::endl;
    std::cout << "  Bleu  coin=(" << x0_bleu  << "," << y0_bleu  << ")" << std::endl;
    std::cout << "  Rouge coin=(" << x0_rouge << "," << y0_rouge << ")" << std::endl;


    std::vector<Particule> ma_collection;
    ma_collection.reserve(nx_bleu*ny_bleu + nx_rouge*ny_rouge);

    // Bloc bleu (immobile)
    for (int i = 0; i < nx_bleu; ++i)
        for (int j = 0; j < ny_bleu; ++j) {
            Vecteur pos(x0_bleu + i*d, y0_bleu + j*d, 0.0);
            Vecteur vit(0.0, 0.0, 0.0);
            Vecteur forc(0.0, 0.0, 0.0);
            ma_collection.push_back(
                Particule(pos, vit, forc, m, (int)ma_collection.size(), "bleu"));
        }

    // Bloc rouge (vitesse initiale vers le bas)
    for (int i = 0; i < nx_rouge; ++i)
        for (int j = 0; j < ny_rouge; ++j) {
            Vecteur pos(x0_rouge + i*d, y0_rouge + j*d, 0.0);
            Vecteur vit(0.0, -10.0, 0.0);
            Vecteur forc(0.0, 0.0, 0.0);
            ma_collection.push_back(
                Particule(pos, vit, forc, m, (int)ma_collection.size(), "rouge"));
        }

    std::cout << "  Total particules : " << ma_collection.size() << std::endl;

 
    UniversTp4 univers(2, Ld, r_cut, sigma, epsilon, ma_collection);
    univers.maj_cellules();   // placement initial

    std::ofstream file("simulation_data.csv");
    file << "t,x,y,couleur\n";

    // save_freq=2000 -> ~195 frames pour t_final=19.5
    int save_freq  = 2000;
    int total_steps = (int)(t_final / dt);
    std::cout << "  Steps : " << total_steps
              << "  Frames attendues : " << total_steps/save_freq << std::endl;

    // Frame initiale t=0
    for (const auto& p : ma_collection)
        file << 0.0 << "," << p.getPosition()[0] << ","
             << p.getPosition()[1] << "," << p.getCat() << "\n";

    // Boucle 
    double t = 0.0;
    int step = 0, frames = 1;
    auto t0 = std::chrono::high_resolution_clock::now();
    std::cout << "\nSimulation en cours..." << std::endl;

    while (t < t_final) {
        univers.avancer(dt);
        t += dt;
        ++step;

        if (step % save_freq == 0) {
            for (const auto& p : ma_collection)
                file << t << "," << p.getPosition()[0] << ","
                     << p.getPosition()[1] << "," << p.getCat() << "\n";
            ++frames;

            double pct = 100.0 * t / t_final;
            double elapsed = std::chrono::duration<double>(
                std::chrono::high_resolution_clock::now() - t0).count();
            std::cout << "\r  " << (int)pct << "%  frames=" << frames
                      << "  elapsed=" << (int)elapsed << "s    " << std::flush;
        }
    }

    file.close();
    double total = std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t0).count();
    std::cout << "\nTermine en " << total << "s  |  "
              << frames << " frames -> simulation_data.csv" << std::endl;
    std::cout << "Lancer : python3 visualiser_tombati.py" << std::endl;

    return 0;
}
