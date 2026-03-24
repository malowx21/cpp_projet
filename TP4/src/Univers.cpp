#include "Univers.hpp"
#include <iostream>
#include <cmath>

Univers::Univers(int dim) : dimension(dim), t(0.0) {}

void Univers::ajouterParticule(const Particule& p) {
    particules.push_back(p);
}

void Univers::calcul_forces() {
    double eps = 0.05;  //parametre de softening
    double G = 1.0;
    int N = particules.size();
    double softening = eps * eps;

    //reinitialisation des forces
    for (auto& p : particules)
        p.setForce(Vecteur(0, 0, 0));

    //calcul des forces gravitationnelles avec Newton 3
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            Vecteur rij = particules[j].getPosition() - particules[i].getPosition();

            double dist = rij.norm();

            // F = G * m1 * m2 / r^2 avec softening
            double factor = G * particules[i].getMasse() * particules[j].getMasse()
                          / (dist * dist * dist + softening);

            Vecteur F = rij * factor;

    // mettre_a_jour_cellules();

    const double rcut2 = rcut * rcut;
    const double sig2  = sigma * sigma;
    const double eps24 = 24.0 * epsilon;

    for (size_t ic = 0; ic < cellules.size(); ic++) {
        if (cellules[ic].est_vide()) continue;

        const Cellule& cell_i = cellules[ic];

        for (int n = 0; n < cell_i.getNbVoisins(); ++n) {
            int jc = cell_i.getVoisins()[n];
            if (jc < (int)ic) continue;

            const Cellule& cell_j = cellules[jc];
            if (cell_j.est_vide()) continue;

            for (int i : cell_i.getParticules()) {
                for (int j : cell_j.getParticules()) {
                    if (ic == jc && i >= j) continue;

                    Vecteur rij = particules[j].getPosition() - particules[i].getPosition();
                    double rx = rij.getX(), ry = rij.getY(), rz = rij.getZ();
                    double r2 = rx*rx + ry*ry + rz*rz;

                    if (r2 < 1e-10 || r2 > rcut2) continue;

                    double sr2    = sig2 / r2;
                    double sr6    = sr2 * sr2 * sr2;
                    double sr12   = sr6 * sr6;
                    double facteur = eps24 / r2 * (sr6 - 2.0 * sr12);

                    Vecteur Fij = rij * facteur;
                    particules[i].getForce() += Fij;
                    particules[j].getForce() = particules[j].getForce() - Fij;
                }
            }
        }
    }
}

void Univers::avancer(double dt, double t_end) {
    if (t >= t_end) return;
    int N = particules.size();
    if (N == 0) return;

    calcul_forces();

    //integration Euler
    for (int i = 0; i < N; i++) {
        //mise a jour vitesse et position
        Vecteur acc = particules[i].getForce() / particules[i].getMasse();
        particules[i].getPosition() += particules[i].getVitesse() * dt
                                     + acc * (0.5 * dt * dt);
    }

    // Q5 : replacer dans les cellules apres deplacement
    mettre_a_jour_cellules();

    // sauvegarde F(t) et calcul F(t+dt)
    std::vector<Vecteur> forces_old(N);
    for (int i = 0; i < N; i++)
        forces_old[i] = particules[i].getForce();

    if (utiliser_gravite) {
        // calculer_forces_gravite();
    } else {
        calculer_forces_lj();
    }

    // mise a jour des vitesses avec F(t) et F(t+dt)
    for (int i = 0; i < N; i++) {
        Vecteur acc_old = forces_old[i] / particules[i].getMasse();
        Vecteur acc_new = particules[i].getForce() / particules[i].getMasse();
        particules[i].getVitesse() += (acc_old + acc_new) * (0.5 * dt);
    }

    t += dt;
}

void Univers::afficher() const {
    for (const auto& p : particules)
        p.AffichePosition();
}
