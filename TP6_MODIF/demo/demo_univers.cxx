#include "Vecteur.hpp"
#include "Particule.hpp"
#include "Univers.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <filesystem>

// -------------------- PARAMS --------------------
const double EPSILON = 1.0;
const double SIGMA   = 1.0;
const double RCUT    = 2.5 * SIGMA;

const double DT    = 0.0005;
const double T_END = 29.5;

const int N1 = 15;    // falling object
const int N2 = 200;  // bottom medium

const double G = -12.0;
const double ECIBLE = 0.005 * (N1 + N2);

// -------------------- VTK EXPORT --------------------
void write_vtk(const Univers& U, int frame) {

    std::string filename = "vtk_output/frame_" + std::to_string(frame) + ".vtk";
    std::ofstream f(filename);

    if (!f.is_open()) {
        std::cerr << "ERROR: cannot open " << filename << "\n";
        return;
    }

    const auto& P = U.get_particules();
    int N = P.size();

    f << "# vtk DataFile Version 3.0\n";
    f << "Particles\nASCII\nDATASET POLYDATA\n";
    f << "POINTS " << N << " float\n";

    for (const auto& p : P)
        f << p.getPosition().getX() << " "
          << p.getPosition().getY() << " "
          << p.getPosition().getZ() << "\n";

    f << "VERTICES " << N << " " << 2*N << "\n";
    for (int i = 0; i < N; i++)
        f << "1 " << i << "\n";

    f.close();

    std::cout << "Saved: " << filename << "\n";
}

void create_disk(Univers& U, double cx, double cy,
                 double radius, double spacing,
                 Vecteur v0, int& id) {

    for (double x = cx - radius; x <= cx + radius; x += spacing) {
        for (double y = cy - radius; y <= cy + radius; y += spacing) {

            double dx = x - cx;
            double dy = y - cy;

            if (dx*dx + dy*dy <= radius*radius) {
                Particule p;
                p.setPosition(Vecteur(x, y, 0));
                p.setVitesse(v0);
                p.setForce(Vecteur(0,0,0));
                p.setMasse(1.0);
                p.setId(id++);
                U.ajouter_particule(p);
            }
        }
    }
}
void create_sea(Univers& U, double width, double height,
                double spacing, int& id) {

    for (double x = 0; x < width; x += spacing) {
        for (double y = 0; y < height; y += spacing) {

            Particule p;
            p.setPosition(Vecteur(x, y, 0));
            p.setVitesse(Vecteur(0, 0, 0));
            p.setForce(Vecteur(0,0,0));
            p.setMasse(1.0);
            p.setId(id++);
            U.ajouter_particule(p);
        }
    }
}
int main() {
    (void)system("rm -rf vtk_output");
    std::filesystem::create_directories("vtk_output");

    Univers U(2, EPSILON, SIGMA, RCUT, {120.0, 120.0, 1.0});

    int id = 0;

    double spacing = 1.3 * SIGMA; // FIXED

    // SEA
    create_sea(U, 120.0, 40.0, spacing, id);

    // BALL
    create_disk(U,
                60.0, 90.0,
                5.0,
                spacing,
                Vecteur(0, -2, 0), // FIXED
                id);

    std::cout << "Particles: " << id << "\n";

    int step = 0;
    int frame = 0;

    while (U.get_temps() < T_END) {

        U.avancer(DT, T_END, true, -3.0); // FIXED

        if (step % 200 == 0)
            U.rescaler_vitesses(0.005 * id);

        if (step > 100 && step % 50 == 0)
            write_vtk(U, frame++);

        step++;
    }

    std::cout << "Done.\n";
}