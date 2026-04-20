#include "Vecteur.hpp"
#include "Particule.hpp"
#include "Univers.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>

// -------------------- PARAMS --------------------
const double EPSILON = 1.0;
const double SIGMA   = 1.0;
const double RCUT    = 2.5 * SIGMA;

const double DT    = 0.0005;
const double T_END = 29.5;

const int N1 = 395;    // falling object
const int N2 = 17227;  // bottom medium

const double G = -12.0;
const double ECIBLE = 0.005 * (N1 + N2);

// -------------------- VTK EXPORT --------------------
void write_vtk(const Univers& U, int frame) {
    std::ofstream f("frame_" + std::to_string(frame) + ".vtk");

    const auto& P = U.get_particules();
    int N = P.size();

    f << "# vtk DataFile Version 3.0\n";
    f << "Particles\nASCII\nDATASET POLYDATA\n";
    f << "POINTS " << N << " float\n";

    for (auto& p : P)
        f << p.getPosition().getX() << " "
          << p.getPosition().getY() << " "
          << p.getPosition().getZ() << "\n";

    f << "VERTICES " << N << " " << 2*N << "\n";
    for (int i = 0; i < N; i++)
        f << "1 " << i << "\n";
}

// -------------------- CREATE BLOCK --------------------
void create_block(Univers& U, int n, double x0, double y0,
                  int nx, int ny, Vecteur v0, int& id) {

    double d = pow(2.0, 1.0/6.0) * SIGMA;

    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            Particule p;
            p.setPosition(Vecteur(x0 + i*d, y0 + j*d, 0));
            p.setVitesse(v0);
            p.setForce(Vecteur(0,0,0));
            p.setMasse(1.0);
            p.setId(id++);
            U.ajouter_particule(p);
        }
    }
}

// -------------------- MAIN --------------------
int main() {

    Univers U(2, EPSILON, SIGMA, RCUT, {250.0, 180.0, 1.0});

    int id = 0;

    // ---------- SEA (N2) ----------
    int nx2 = 130;
    int ny2 = N2 / nx2;

    create_block(U, N2, 0.0, 0.0, nx2, ny2, Vecteur(0,0,0), id);

    // ---------- FALLING OBJECT (N1) ----------
    int nx1 = 20;
    int ny1 = N1 / nx1;

    create_block(U, N1, 80.0, 120.0, nx1, ny1, Vecteur(0,10,0), id);

    std::cout << "Total particles: " << id << "\n";

    // ---------- SIMULATION ----------
    int step = 0;
    int frame = 0;

    while (U.get_temps() < T_END) {

        U.avancer(DT, T_END, true, G);

        // 🔥 ENERGY CONTROL (IMPORTANT QUESTION)
        if (step % 1000 == 0) {
            U.rescaler_vitesses(ECIBLE);
        }

        // VTK export
        if (step % 500 == 0) {
            write_vtk(U, frame++);
            std::cout << "t=" << U.get_temps() << "\n";
        }

        step++;
    }

    std::cout << "Simulation finished\n";
}