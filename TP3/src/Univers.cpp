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

            particules[i].getForce() += F;
            particules[j].getForce() -= F;  //par la 3eme loi de Newton
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
        particules[i].getVitesse()  += acc * dt;
        particules[i].getPosition() += particules[i].getVitesse() * dt;
    }

    t += dt;
}

void Univers::afficher() const {
    for (const auto& p : particules)
        p.AffichePosition();
}
