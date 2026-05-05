#include "Univers.hpp"
#include <iostream>
#include <algorithm>
#include <vector>


// Constructeur : crée l'univers de simulation et l'initialise
// avec ses paramètres physiques et géomètriques .

Univers::Univers(int dim, double eps, double sig, double r_cut,std::vector<double> longueurs, int type)
    : dimension(dim), t(0.0), epsilon(eps), sigma(sig),rcut(r_cut), L(std::move(longueurs)), type_border(type) {
    while (L.size() < 3) L.push_back(1.0);
    // Calcule du nombre et taille de cellules dans chaque directions 
    for (int d = 0; d < 3; d++) {
        n_cubes[d] = std::max(1, static_cast<int>(L[d] / rcut));
        taille_cellule[d] = L[d] / n_cubes[d];
    }
    initialiser_grille();
}

// Condition aux limites : applique les conditions de réflexion, absorption 
// et périodique à toutes les particules .
void Univers::appliquer_conditions_limites() {

    const double eps_wall = 1e-6;
    for (auto& p : particules) {
        // Références directes — aucune copie Vecteur
        Vecteur& pos = p.getPosition();
        Vecteur& vit = p.getVitesse();
        // Selon l'axe X
        if (pos.getX() < 0.0) {
            if (type_border == 0) {
                pos.setX(eps_wall);
                vit.setX(std::abs(vit.getX()));
            }
            else if (type_border == 1) {
                pos.setX(pos.getX() + L[0]);
            }
            else if (type_border == 2) {
                p.setMasse(0);
            }
        }
        else if (pos.getX() > L[0]) {
            if (type_border == 0) {
                pos.setX(L[0] - eps_wall);
                vit.setX(-std::abs(vit.getX()));
            }
            else if (type_border == 1) {
                pos.setX(pos.getX() - L[0]);
            }
            else if (type_border == 2) {
                p.setMasse(0);
            }
        }

        // Selon l'axe Y
        if (pos.getY() < 0.0) {
            if (type_border == 0) {
                pos.setY(eps_wall);
                vit.setY(std::abs(vit.getY()));
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
                pos.setY(L[1] - eps_wall);
                vit.setY(-std::abs(vit.getY()));
            }
            else if (type_border == 1) {
                pos.setY(pos.getY() - L[1]);
            }
            else if (type_border == 2) {
                p.setMasse(0);
            }
        }

        // Selon l'axe Z
        if (pos.getZ() < 0.0) {
            if (type_border == 0) {
                pos.setZ(eps_wall);
                vit.setZ(std::abs(vit.getZ()));
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
                pos.setZ(L[2] - eps_wall);
                vit.setZ(-std::abs(vit.getZ()));
            }
            else if (type_border == 1) {
                pos.setZ(pos.getZ() - L[2]);
            }
            else if (type_border == 2) {
                p.setMasse(0);
            }
        }
    }

    // Suppression si absorption
    if (type_border == 2) {
        particules.erase(
            std::remove_if(particules.begin(), particules.end(),
                [](const Particule& p) { return p.getMasse() == 0; }),
            particules.end()
        );
    }
}
// Initialisation de la grille des cellules de l'univers 
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


// Ajout une particule à la liste des particules de l'univers
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

// Retourne liste des indices des cellules voisines d'une cellule selon son indice
std::vector<int> Univers::get_voisins(int cellule_idx) const {
    std::vector<int> res;
    const Cellule& c = cellules[cellule_idx];
    // Boucle sur le nombre de voisin de la cellule
    for (int n = 0; n < c.getNbVoisins(); ++n)
        res.push_back(c.getVoisins()[n]);
    return res;
}

// Ajoute force de repulsion sur chaque particule qui s'approche des parois
void Univers::ajouter_forces_parois() {
    const double A_cut = std::pow(2.0, 1.0/6.0) * sigma;

    // Fonction lambda pour calculer les forces de Lennard-Jones pour chaque cas 
    auto lj_wall = [&](double d) -> double {
        double A   = std::max(d, 0.1 * sigma);
        double invA = 1.0 / A;
        double sr  = sigma * invA;
        double sr6 = sr * sr; sr6 = sr6 * sr6 * sr6;
        return -24.0 * epsilon * invA * sr6 * (1.0 - 2.0 * sr6);
    };

    for (auto& p : particules) {
        Vecteur& pos = p.getPosition();  
        Vecteur& force= p.getForce();
        const double px = pos.getX();
        const double py = pos.getY();

        if (px < A_cut) force.setX(force.getX() + lj_wall(px));
        if (L[0] - px < A_cut) force.setX(force.getX() - lj_wall(L[0] - px));
        if (py < A_cut) force.setY(force.getY() + lj_wall(py));
        if (L[1] - py < A_cut) force.setY(force.getY() - lj_wall(L[1] - py));
    }
}

// Calcule des forces de lennard-jonnes entre toutes les deux particules 
// le rayoin est inférieur à rcut 
void Univers::calculer_forces_lj() {
    // On met toutes les forces à 0 
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
            // Chaque pair de particule est traitée une seul fois 
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
                    // Eviter les doublons  ainsi que l'auto-interaction 
                    if (ic == (size_t)jc && i >= j) continue;
                    Particule& pj = particules[j];
                    const Vecteur& posj = pj.getPosition();
                    Vecteur& Fj = pj.getForce();

                    double rx = posj.getX() - posi.getX();
                    double ry = posj.getY() - posi.getY();
                    double rz = posj.getZ() - posi.getZ();

                    double r2 = rx*rx + ry*ry + rz*rz;
                    
                    // Eviter la divergence si dexu particules sont au meme endroit
                    if (r2 < 0.25 * sigma * sigma) r2 = 0.25 * sigma * sigma;
                    // On ignore les pairs dont le rayon est supérieur à rcut 
                    if (r2 >rcut2) continue;
                    
                    double inv_r2 = 1.0 / r2;
                    double sr2 = sig2 * inv_r2;
                    double sr6 = sr2 * sr2 * sr2;
                    
                    double f = eps24 * inv_r2 * sr6*(1.0 - 2.0 * sr6);
                    // Vecteur force entre les particules i et j
                    Vecteur Fij(rx * f, ry * f, rz * f);

                    Fi += Fij;
                    // Troisième loi de Newton 
                    Fj -= Fij;
                }
            }
        }
    }
    // Ajout des forces de répulsion des parois 
    ajouter_forces_parois();
}

// Energie cinetique du systeme de particules 
double Univers::energie_cinetique() const {
    double Ec = 0.0;
    for (const auto& p : particules) {
        const Vecteur& v = p.getVitesse();
        const double vx  = v.getX(), vy = v.getY(), vz = v.getZ();
        Ec += 0.5 * p.getMasse() * (vx*vx + vy*vy + vz*vz);
    }
    return Ec;
}

// Mise en echelle des vitesses pour que l'énegie cinétique 
// atteigne l'énergie cible afin de limiter la divergence de la vitesse.
void Univers::rescaler_vitesses(double Ec_cible) {
    const double Ec = energie_cinetique();
    if (Ec < 1e-12) {
        std::cerr << " Energie cinetique quasi-nulle, rescaling ignoré\n";
        return;
    }
    const double lambda = std::sqrt(Ec_cible / Ec);
    for (auto& p : particules) {
        Vecteur& v = p.getVitesse();
        v.setX(v.getX() * lambda);
        v.setY(v.getY() * lambda);
        v.setZ(v.getZ() * lambda);
    }
}
// Ajout de la force de gravitationnelle à chaque 
// particule  notamment  pour la composante suivant Y
void Univers::ajouter_gravite(double g) {
    for (auto& p : particules) {
        Vecteur& f = p.getForce();
        f.setY(f.getY() + p.getMasse() * g);
        
    }
}

//  Avancement de l'état de la grille des cellules  dans le temps de dt selon l'algorithme de Verlet  
void Univers::avancer(double dt, double t_end, bool utiliser_gravite, double g) {

    if (t >= t_end) return;
    const int N = static_cast<int>(particules.size());
    // Forces à t
    calculer_forces_lj();
    if (utiliser_gravite) ajouter_gravite(g);
    // Positions — pas de Vecteur temporaire
    for (int i = 0; i < N; i++) {
        Particule& p = particules[i];
        const double inv_m = 1.0 / p.getMasse();
        const double c     = 0.5 * dt * dt * inv_m;

        Vecteur& pos = p.getPosition();
        const Vecteur& vel = p.getVitesse();
        const Vecteur& f = p.getForce();

        pos.setX(pos.getX() + vel.getX() * dt + f.getX() * c);
        pos.setY(pos.getY() + vel.getY() * dt + f.getY() * c);
        pos.setZ(pos.getZ() + vel.getZ() * dt + f.getZ() * c);
    }

    appliquer_conditions_limites();
    mettre_a_jour_cellules();

    // Sauvegarde des forces à t, variable locale (pas static)
    std::vector<Vecteur> forces_old(N);
    for (int i = 0; i < N; i++)
        forces_old[i] = particules[i].getForce();

    // Forces à t+dt
    calculer_forces_lj();
    if (utiliser_gravite) ajouter_gravite(g);

    // Vitesses — pas de Vecteur temporaire
    for (int i = 0; i < N; i++) {
        Particule& p = particules[i];
        const double inv_m = 1.0 / p.getMasse();
        const double c = 0.5 * dt * inv_m;

        Vecteur& vel  = p.getVitesse();
        const Vecteur& f_old = forces_old[i];
        const Vecteur& f_new  = p.getForce();

        vel.setX(vel.getX() + (f_old.getX()+f_new.getX()) * c);
        vel.setY(vel.getY() + (f_old.getY()+f_new.getY()) * c);
        vel.setZ(vel.getZ() + (f_old.getZ()+f_new.getZ()) * c);
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


// Vérifie que l'état de la simulation est physiquement valide. Sinon lève des exceptions
void Univers::check_validite() {
    for (const auto& p : particules) {
        const Vecteur& pos = p.getPosition();   // référence, pas de copie
        if (!std::isfinite(pos.getX()) ||!std::isfinite(pos.getY()) ||!std::isfinite(pos.getZ()))
            throw std::runtime_error("Position non finie détectée, explosion numérique probable");
    }
    if (!std::isfinite(energie_cinetique()))
        throw std::runtime_error("Énergie cinétique non finie, explosion numérique probable");
}
