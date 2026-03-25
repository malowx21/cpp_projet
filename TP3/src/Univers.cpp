#include "Univers.hpp"
#include <iostream>
#include <cmath>
#include <vector>

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

//Stormer-Verlet (vitesse-Verlet)

void Univers::avancer(double dt, double t_end) {
    if (t >= t_end) return;
    int N = particules.size();
    if (N == 0) return;

    //F(t)
    calcul_forces();

    // mise a jour des positions avec F(t)
    for (int i = 0; i < N; i++) {
        Vecteur acc = particules[i].getForce() / particules[i].getMasse();
        particules[i].getPosition() += particules[i].getVitesse() * dt
                                     + acc * (0.5 * dt * dt);
    }

    // sauvegarde F(t) et calcul F(t+dt)
    std::vector<Vecteur> forces_old(N);
    for (int i = 0; i < N; i++)
        forces_old[i] = particules[i].getForce();

    calcul_forces();

    // mise a jour des vitesses avec F(t) et F(t+dt)
    for (int i = 0; i < N; i++) {
        Vecteur acc_old = forces_old[i]            / particules[i].getMasse();
        Vecteur acc_new = particules[i].getForce() / particules[i].getMasse();
        particules[i].getVitesse() += (acc_old + acc_new) * (0.5 * dt);
    }

    t += dt;
}

void Univers::afficher() const {
    for (const auto& p : particules)
        p.AffichePosition();
}
