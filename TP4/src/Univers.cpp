#include "Univers.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

Univers::Univers(int dim, double eps, double sig, double r_cut, std::vector<double> longueurs)
    : dimension(dim), t(0.0), epsilon(eps), sigma(sig), rcut(r_cut), L(longueurs) {

    while (L.size() < 3) L.push_back(1.0);

    for (int d = 0; d < 3; d++) {
        n_cubes[d] = std::max(1, static_cast<int>(L[d] / rcut));
        taille_cellule[d] = L[d] / n_cubes[d];
    }

    std::cout << "Grille: " << n_cubes[0] << " x " << n_cubes[1] << " x " << n_cubes[2]
              << " = " << n_cubes[0]*n_cubes[1]*n_cubes[2] << " cellules\n";

    initialiser_grille();
}

void Univers::initialiser_grille() {
    int total = n_cubes[0] * n_cubes[1] * n_cubes[2];
    cellules.resize(total);

    for (int i = 0; i < n_cubes[0]; i++) {
        for (int j = 0; j < n_cubes[1]; j++) {
            for (int k = 0; k < n_cubes[2]; k++) {
                int idx = i + j*n_cubes[0] + k*n_cubes[0]*n_cubes[1];

                cellules[idx].setCentre(Vecteur(
                    (i + 0.5) * taille_cellule[0],
                    (j + 0.5) * taille_cellule[1],
                    (k + 0.5) * taille_cellule[2]
                ));
                cellules[idx].setIndex(idx);

                // calcul des voisins valides dans les 27 directions
                int voisin_idx = 0;
                for (int di = -1; di <= 1; di++)
                    for (int dj = -1; dj <= 1; dj++)
                        for (int dk = -1; dk <= 1; dk++) {
                            int ni = i+di, nj = j+dj, nk = k+dk;
                            if (ni >= 0 && ni < n_cubes[0] &&
                                nj >= 0 && nj < n_cubes[1] &&
                                nk >= 0 && nk < n_cubes[2])
                                cellules[idx].setVoisin(voisin_idx++,
                                    ni + nj*n_cubes[0] + nk*n_cubes[0]*n_cubes[1]);
                        }
                cellules[idx].setNbVoisins(voisin_idx);
            }
        }
    }
}

void Univers::ajouter_particule(const Particule& p) {
    particules.push_back(p);
}

int Univers::get_cellule_index(const Vecteur& pos) const {
    int i = static_cast<int>(pos.getX() / taille_cellule[0]);
    int j = static_cast<int>(pos.getY() / taille_cellule[1]);
    int k = static_cast<int>(pos.getZ() / taille_cellule[2]);

    i = std::max(0, std::min(i, n_cubes[0] - 1));
    j = std::max(0, std::min(j, n_cubes[1] - 1));
    k = std::max(0, std::min(k, n_cubes[2] - 1));

    return i + j*n_cubes[0] + k*n_cubes[0]*n_cubes[1];
}

void Univers::mettre_a_jour_cellules() {
    for (auto& c : cellules) c.vider();
    for (size_t i = 0; i < particules.size(); i++)
        cellules[get_cellule_index(particules[i].getPosition())].addParticule(i);
}

std::vector<int> Univers::get_voisins(int cellule_idx) const {
    std::vector<int> res;
    const Cellule& c = cellules[cellule_idx];
    for (int n = 0; n < c.getNbVoisins(); ++n)
        res.push_back(c.getVoisins()[n]);
    return res;
}

void Univers::calculer_forces_lj() {
    for (auto& p : particules) p.setForce(Vecteur(0, 0, 0));

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

// Stormer-Verlet (vitesse-Verlet)
// x(t+dt) = x(t) + v(t)*dt + 0.5*a(t)*dt²
// v(t+dt) = v(t) + 0.5*(a(t) + a(t+dt))*dt
void Univers::avancer(double dt, double t_end, bool utiliser_gravite) {
    if (t >= t_end) return;

    int N = particules.size();

    // F(t)
    if (utiliser_gravite) {
        // calculer_forces_gravite();
    } else {
        calculer_forces_lj();
    }

    // mise a jour des positions avec F(t)
    for (int i = 0; i < N; i++) {
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

void Univers::afficher_stats_grille() const {
    int occupees = 0;
    size_t max_p = 0;
    for (const auto& c : cellules) {
        if (!c.est_vide()) {
            ++occupees;
            max_p = std::max(max_p, c.getParticules().size());
        }
    }
    std::cout << "Cellules occupees: " << occupees << "/" << cellules.size()
              << " (" << (100.0 * occupees / cellules.size()) << "%)\n";
    std::cout << "Max particules/cellule: " << max_p << "\n";
}
