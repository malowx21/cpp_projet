#include "Univers.hpp"
#include <iostream>
#include <algorithm>
#include <vector>


// Constructeur 
Univers::Univers(int dim, double eps, double sig, double r_cut, std::vector<double> longueurs)
    : dimension(dim), t(0.0), epsilon(eps), sigma(sig), rcut(r_cut), L(longueurs)  {

    while (L.size() < 3) L.push_back(1.0);
    
    // Calcule du nombre et taille de cellules dans chaque directions 
    for (int d = 0; d < 3; d++) {
        n_cubes[d] = std::max(1, static_cast<int>(L[d] / rcut));
        taille_cellule[d] = L[d] / n_cubes[d];
    }
    // std::cout << "Grille: " << n_cubes[0] << " x " << n_cubes[1] << " x " << n_cubes[2]
    //           << " = " << n_cubes[0]*n_cubes[1]*n_cubes[2] << " cellules\n";

    // Initialisation de la grille des cellules 
    initialiser_grille();
}
void Univers::appliquer_conditions_limites() {

    for (auto& p : particules) {

        Vecteur& pos = p.getPosition();
        Vecteur& vit = p.getVitesse();

        //  X direction 
        if (pos.getX() < 0) {

            if (type_border == 0) { // REFLECTIVE
                pos.setX(0);
                vit.setX(-vit.getX());
            }
            else if (type_border == 1) { // PERIODIC
                pos.setX(pos.getX() + L[0]);
            }
            else if (type_border == 2) { // ABSORBING
                p.setMasse(0); // mark for deletion
            }
        }
        else if (pos.getX() > L[0]) {

            if (type_border == 0) { // REFLECTIVE
                pos.setX(L[0]);
                vit.setX(-vit.getX());
            }
            else if (type_border == 1) { // PERIODIC
                pos.setX(pos.getX() - L[0]);
            }
            else if (type_border == 2) {
                p.setMasse(0);
            }
        }

        //  Y direction 
        if (pos.getY() < 0) {

            if (type_border == 0) {
                pos.setY(0);
                vit.setY(-vit.getY());
            }
            else if (type_border == 1) {
                pos.setY(pos.getY() + L[1]);
            }
            else if (type_border == 2) {
                p.setMasse(0);
            }
        }
        else if (pos.getY() > L[1]) {

            if (type_border == 0) {
                pos.setY(L[1]);
                vit.setY(-vit.getY());
            }
            else if (type_border == 1) {
                pos.setY(pos.getY() - L[1]);
            }
            else if (type_border == 2) {
                p.setMasse(0);
            }
        }

        //  Z direction 
        if (pos.getZ() < 0) {

            if (type_border == 0) {
                pos.setZ(0);
                vit.setZ(-vit.getZ());
            }
            else if (type_border == 1) {
                pos.setZ(pos.getZ() + L[2]);
            }
            else if (type_border == 2) {
                p.setMasse(0);
            }
        }
        else if (pos.getZ() > L[2]) {

            if (type_border == 0) {
                pos.setZ(L[2]);
                vit.setZ(-vit.getZ());
            }
            else if (type_border == 1) {
                pos.setZ(pos.getZ() - L[2]);
            }
            else if (type_border == 2) {
                p.setMasse(0);
            }
        }

        p.setPosition(pos);
        p.setVitesse(vit);
    }

    //  Remove absorbed particles 
    if (type_border == 2) {
        particules.erase(
            std::remove_if(particules.begin(), particules.end(),
                [](const Particule& p) {
                    return p.getMasse() == 0;
                }),
            particules.end()
        );
    }
}
void Univers::initialiser_grille() {
    // Nombre totale des cellules 
    int total = n_cubes[0] * n_cubes[1] * n_cubes[2];
    cellules.resize(total);

    for (int i = 0; i < n_cubes[0]; i++) {
        for (int j = 0; j < n_cubes[1]; j++) {
            for (int k = 0; k < n_cubes[2]; k++) {
                int idx = i + j*n_cubes[0] + k*n_cubes[0]*n_cubes[1];

                cellules[idx].setCentre(Vecteur((i + 0.5) * taille_cellule[0],
                    (j + 0.5) * taille_cellule[1],(k + 0.5) * taille_cellule[2]));
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


// Ajout de la particule
void Univers::ajouter_particule(const Particule& p) {
    particules.push_back(p);
}

// Obtenir l'indice de la cellule selon la position 
int Univers::get_cellule_index(const Vecteur& pos) const {
    int i = static_cast<int>(pos.getX() / taille_cellule[0]);
    int j = static_cast<int>(pos.getY() / taille_cellule[1]);
    int k = static_cast<int>(pos.getZ() / taille_cellule[2]);
    i = std::max(0, std::min(i, n_cubes[0] - 1));
    j = std::max(0, std::min(j, n_cubes[1] - 1));
    k = std::max(0, std::min(k, n_cubes[2] - 1));
    return i + j*n_cubes[0] + k*n_cubes[0]*n_cubes[1];
}

// Mise à jour des cellules 
void Univers::mettre_a_jour_cellules() {
    // Vider toutes les cellules 
    for (auto& c : cellules) c.vider();
    // Remplir avec le nouveau état des particules
    for (size_t i = 0; i < particules.size(); i++)
        cellules[get_cellule_index(particules[i].getPosition())].addParticule(i);
}

// Obtenir les voisins d'une cellule selon son indice
std::vector<int> Univers::get_voisins(int cellule_idx) const {
    std::vector<int> res;
    const Cellule& c = cellules[cellule_idx];
    // Boucle sur le nombre de voisin de la cellule
    for (int n = 0; n < c.getNbVoisins(); ++n)
        res.push_back(c.getVoisins()[n]);
    return res;
}
void Univers::ajouter_forces_parois() {

    const double A_cut = pow(2.0, 1.0/6.0) * sigma;

    for (auto& p : particules) {

        Vecteur pos = p.getPosition();
        Vecteur force = p.getForce();

        // Distance to walls
        double d_left   = pos.getX();
        double d_right  = L[0] - pos.getX();
        double d_bottom = pos.getY();
        double d_top    = L[1] - pos.getY();

        // Example: LEFT wall
        if (d_left < A_cut) {
            double A = d_left;
            double ratio = sigma / (2*A);
            double r2 = ratio * ratio;
            double r6 = r2 * r2 * r2;
            double f = -24 * epsilon / (2*A) * r6 * (1 - 2*r6);

            force.setX(force.getX() + f);
        }

        // RIGHT wall
        if (d_right < A_cut) {
            double A = d_right;
            double ratio = sigma / (2*A);
            double r2 = ratio * ratio;
            double r6 = r2 * r2 * r2;

            double f = 24 * epsilon / (2*A) * r6 * (1 - 2*r6);

            force.setX(force.getX() - f);
        }

        // SAME for Y walls...
        // BOTTOM
        if (d_bottom < A_cut) {
            double A = d_bottom;
            double ratio = sigma / (2*A);
            double r2 = ratio * ratio;
            double r6 = r2 * r2 * r2;
            double f = -24 * epsilon / (2*A) * r6 * (1 - 2*r6);
            force.setY(force.getY() + f);
        }

        // TOP
        if (d_top < A_cut) {
            double A = d_top;
            double ratio = sigma / (2*A);
            double r2 = ratio * ratio;
            double r6 = r2 * r2 * r2;
            double f = 24 * epsilon / (2*A) * r6 * (1 - 2*r6);
            force.setY(force.getY() - f);
        }
        p.setForce(force);
    }
}
void Univers::calculer_forces_lj() {

    for (auto& p : particules){
        Vecteur& f = p.getForce();
        f.setX(0); f.setY(0); f.setZ(0);
    }
    const double rcut2 = rcut * rcut;
    const double sig2  = sigma * sigma;
    const double eps24 = 24.0 * epsilon;

    for (size_t ic = 0; ic < cellules.size(); ic++) {

        const Cellule& cell_i = cellules[ic];
        if (cell_i.est_vide()) continue;

        const auto& voisins_i = cell_i.getVoisins();
        int nb_voisins_i = cell_i.getNbVoisins();

        for (int n = 0; n < nb_voisins_i; ++n) {

            int jc = voisins_i[n];
            if (jc < (int)ic) continue;

            const Cellule& cell_j = cellules[jc];
            if (cell_j.est_vide()) continue;

            const auto& part_i = cell_i.getParticules();
            const auto& part_j = cell_j.getParticules();

            for (int i : part_i) {

                Particule& pi = particules[i];
                const Vecteur& posi = pi.getPosition();
                Vecteur& Fi = pi.getForce();

                for (int j : part_j) {

                    if (ic == (size_t)jc && i >= j) continue;

                    Particule& pj = particules[j];
                    const Vecteur& posj = pj.getPosition();
                    Vecteur& Fj = pj.getForce();

                    double rx = posj.getX() - posi.getX();
                    double ry = posj.getY() - posi.getY();
                    double rz = posj.getZ() - posi.getZ();

                    double r2 = rx*rx + ry*ry + rz*rz;
                    if (r2 < 1e-12 || r2 > rcut2) continue;

                    

                    double inv_r2 = 1.0 / r2;
                    double sr2 = sig2 * inv_r2;
                    double sr6 = sr2 * sr2 * sr2;
                    

                    double f = eps24 * inv_r2 * sr6*(1.0 - 2.0 * sr6);

                    Vecteur Fij(rx * f, ry * f, rz * f);

                    Fi += Fij;
                    Fj -= Fij;//3 ème loi 
                }
            }
        }
    }

    ajouter_forces_parois();
}

double Univers::energie_cinetique() const {
    double Ec = 0.0;
    for (const auto& p : particules) {
        double v2 = p.getVitesse().norm();
        Ec += 0.5 * p.getMasse() * v2;
    }
    return Ec;
}
void Univers::rescaler_vitesses(double Ec_cible) {
    double Ec = energie_cinetique();
    if(Ec < 1e-12) {std::cout<<"Danger"<<std::endl;
        return;
    }
    double lambda = sqrt(Ec_cible / Ec);

    for (auto& p : particules) {
        p.setVitesse(p.getVitesse() * lambda);
    }
}
void Univers::ajouter_gravite(double g) {
    for (auto& p : particules) {
        Vecteur& f = p.getForce();
        f.setY(f.getY() + p.getMasse() * g);
        p.setForce(f);
    }
}
//  Avancement de l'état de la grille dans le temps de dt 
void Univers::avancer(double dt, double t_end, bool utiliser_gravite, double g) {

    if (t >= t_end) return;

    int N = particules.size();

   

    // --- Forces at t
    calculer_forces_lj();
    if (utiliser_gravite)
        ajouter_gravite(g);

    // --- Positions
    for (int i = 0; i < N; i++) {
        Particule& p = particules[i];

        Vecteur acc = p.getForce() / p.getMasse();

        p.getPosition() += p.getVitesse() * dt
                         + acc * (0.5 * dt * dt);
    }

    appliquer_conditions_limites();

    // --- Update cells AFTER move
    mettre_a_jour_cellules();

    // --- Store old forces
    static std::vector<Vecteur> forces_old;
    forces_old.resize(N);

    for (int i = 0; i < N; i++)
        forces_old[i] = particules[i].getForce();

    // --- Forces at t+dt
    calculer_forces_lj();
    if (utiliser_gravite)
        ajouter_gravite(g);

    // --- Velocities
    for (int i = 0; i < N; i++) {
        Particule& p = particules[i];

        Vecteur acc_old = forces_old[i] / p.getMasse();
        Vecteur acc_new = p.getForce() / p.getMasse();

        p.getVitesse() += (acc_old + acc_new) * (0.5 * dt);
    }

    t += dt;
}
// Afficher l'état de la grille
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