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

        Vecteur pos = p.getPosition();
        Vecteur vit = p.getVitesse();

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
            double r6 = pow(ratio, 6);

            double f = -24 * epsilon / (2*A) * r6 * (1 - 2*r6);

            force.setX(force.getX() + f);
        }

        // RIGHT wall
        if (d_right < A_cut) {
            double A = d_right;
            double ratio = sigma / (2*A);
            double r6 = pow(ratio, 6);

            double f = 24 * epsilon / (2*A) * r6 * (1 - 2*r6);

            force.setX(force.getX() - f);
        }

        // SAME for Y walls...
        // BOTTOM
        if (d_bottom < A_cut) {
            double A = d_bottom;
            double ratio = sigma / (2*A);
            double r6 = pow(ratio, 6);
            double f = -24 * epsilon / (2*A) * r6 * (1 - 2*r6);
            force.setY(force.getY() + f);
        }

        // TOP
        if (d_top < A_cut) {
            double A = d_top;
            double ratio = sigma / (2*A);
            double r6 = pow(ratio, 6);
            double f = 24 * epsilon / (2*A) * r6 * (1 - 2*r6);
            force.setY(force.getY() - f);
        }
        p.setForce(force);
    }
}
void Univers::calculer_forces_lj() {
    for (auto& p : particules) p.setForce(Vecteur(0, 0, 0));

    // mettre_a_jour_cellules();

    // Calcul de quelques paramètres nécessaires dans différentes expressions
    const double rcut2 = rcut * rcut;
    const double sig2  = sigma * sigma;
    const double eps24 = 24.0 * epsilon;

    // Boucles sur les cellules
    for (size_t ic = 0; ic < cellules.size(); ic++) {
        if (cellules[ic].est_vide()) continue;
        const Cellule& cell_i = cellules[ic];
        // Boucle sur les voisns de chaque cellule
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
                    // Calcul de la force  (Lennard-Jones) en la cellule i et sa cellule voisine j 
                    Vecteur Fij = rij * facteur;
                    particules[i].getForce() += Fij;
                    particules[j].getForce() = particules[j].getForce() - Fij;
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
    double lambda = sqrt(Ec_cible / Ec);

    for (auto& p : particules) {
        p.setVitesse(p.getVitesse() * lambda);
    }
}
void Univers::ajouter_gravite(double g) {
    for (auto& p : particules) {
        Vecteur f = p.getForce();
        f.setY(f.getY() + p.getMasse() * g);
        p.setForce(f);
    }
}
//  Avancement de l'état de la grille dans le temps de dt 
void Univers::avancer(double dt, double t_end,bool utiliser_gravite , double g) {
    if (t >= t_end) return;
    int N = particules.size();

    // Calcule de force 
    
    calculer_forces_lj();
    if (utiliser_gravite) {
        ajouter_gravite(-g);
    }

    // Mise à jour des positions avec les nouvelles forces
    for (int i = 0; i < N; i++) {
        Vecteur acc = particules[i].getForce() / particules[i].getMasse();
        particules[i].getPosition() += particules[i].getVitesse() * dt
                                     + acc * (0.5 * dt * dt);
        
    }
    appliquer_conditions_limites();
    // Mise à jour des cellules apres deplacement
    mettre_a_jour_cellules();
    // sauvegarde de la force à l'instant t  et calcul de la force à l'instant t+dt
    std::vector<Vecteur> forces_old(N);
    for (int i = 0; i < N; i++){
        forces_old[i] = particules[i].getForce();
    }
    calculer_forces_lj();
    if (utiliser_gravite) {
        ajouter_gravite(-g);
    }

    // mise a jour des vitesses 
    for (int i = 0; i < N; i++) {
        Vecteur acc_old = forces_old[i] / particules[i].getMasse();
        Vecteur acc_new = particules[i].getForce() / particules[i].getMasse();
        particules[i].getVitesse() += (acc_old + acc_new) * (0.5 * dt);
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