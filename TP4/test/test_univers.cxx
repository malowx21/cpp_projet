#include "Univers.hpp"
#include <cmath>
#include <iostream>
#include <vector>

static bool approx(double a, double b, double tol = 1e-9) {
    return std::abs(a - b) < tol;
}

static void check(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "FAIL : " << msg << "\n";
        std::exit(1);
    }
    std::cout << "  OK : " << msg << "\n";
}

static Particule make_particule(int id, double x, double y, double z,
                                double masse = 1.0) {
    Particule p;
    p.setId(id);
    p.setMasse(masse);
    p.setPosition(Vecteur(x, y, z));
    p.setVitesse(Vecteur(0, 0, 0));
    p.setForce(Vecteur(0, 0, 0));
    return p;
}


void test_grille_dimensions() {
    std::cout << "[Univers] dimensions de la grille\n";

    // domaine 10x10x1, rcut=2.5 => 4x4x1 = 16 cellules
    Univers U(2, 1.0, 1.0, 2.5, {10.0, 10.0, 1.0});

    check(U.get_dimension() == 2, "dimension == 2");

    // Vérification via get_cellule_index sur les coins
    int idx_origine = U.get_cellule_index(Vecteur(0.1, 0.1, 0.0));
    check(idx_origine == 0, "coin origine -> cellule 0");

    int idx_max = U.get_cellule_index(Vecteur(9.9, 9.9, 0.0));
    check(idx_max == 15, "coin max -> cellule 15  (4x4-1)");
}


void test_affectation_cellule() {
    std::cout << "[Univers] affectation cellule\n";

    // domaine 10x10x1, rcut=2.5 => taille_cellule = 2.5
    Univers U(2, 1.0, 1.0, 2.5, {10.0, 10.0, 1.0});

    // (1.0, 1.0) => i=0, j=0 => index 0
    check(U.get_cellule_index(Vecteur(1.0, 1.0, 0.0)) == 0,
          "particule (1,1,0) -> cellule 0");

    // (6.0, 7.5) => i=2, j=3 => index = 2 + 3*4 = 14
    check(U.get_cellule_index(Vecteur(6.0, 7.5, 0.0)) == 14,
          "particule (6,7.5,0) -> cellule 14");

    // Après ajout + mise à jour, la cellule 0 doit être dans ses propres voisins
    U.ajouter_particule(make_particule(0, 1.0, 1.0, 0.0));
    U.mettre_a_jour_cellules();

    auto voisins = U.get_voisins(0);
    bool contient_0 = false;
    for (int v : voisins) if (v == 0) contient_0 = true;
    check(contient_0, "cellule 0 est dans ses propres voisins");
}


void test_voisins() {
    std::cout << "[Univers] voisins de cellules\n";

    // grille 3x3x1 => taille_cellule = 3.0
    Univers U(2, 1.0, 1.0, 3.0, {9.0, 9.0, 1.0});

    // Cellule centrale (1,1,0) -> index 4 : 9 voisins
    auto v_centre = U.get_voisins(4);
    check((int)v_centre.size() == 9, "cellule centrale: 9 voisins");

    // Coin (0,0,0) -> index 0 : 4 voisins en 2D
    auto v_coin = U.get_voisins(0);
    check((int)v_coin.size() == 4, "cellule coin: 4 voisins");

    // Bord milieu (1,0,0) -> index 1 : 6 voisins en 2D
    auto v_bord = U.get_voisins(1);
    check((int)v_bord.size() == 6, "cellule bord milieu: 6 voisins");
}


void test_conservation_qm() {
    std::cout << "[Univers] conservation de la quantite de mouvement\n";

    Univers U(2, 1.0, 1.0, 2.5, {20.0, 20.0, 1.0});

    Particule p0 = make_particule(0, 9.5, 10.0, 0.0);
    p0.setVitesse(Vecteur(1.0, 0, 0));

    Particule p1 = make_particule(1, 10.5, 10.0, 0.0);
    p1.setVitesse(Vecteur(-1.0, 0, 0));

    U.ajouter_particule(p0);
    U.ajouter_particule(p1);
    U.mettre_a_jour_cellules();

    auto qm_x = [&]() {
        double s = 0;
        for (const auto& p : U.get_particules())
            s += p.getMasse() * p.getVitesse().getX();
        return s;
    };
    double qm_init = qm_x();

    for (int i = 0; i < 10; ++i)
        U.avancer(0.0001, 1.0, false);

    check(approx(qm_init, qm_x(), 1e-8),
          "QM conservee apres 10 pas (Newton 3)");
}


void test_ajout_particules() {
    std::cout << "[Univers] ajout de particules\n";

    Univers U(2, 1.0, 1.0, 2.5, {20.0, 20.0, 1.0});
    check(U.get_particules().empty(), "univers vide au depart");

    U.ajouter_particule(make_particule(0, 5.0, 5.0, 0.0));
    U.ajouter_particule(make_particule(1, 10.0, 10.0, 0.0));
    check((int)U.get_particules().size() == 2, "2 particules apres 2 ajouts");
}

int main() {
    test_grille_dimensions();
    test_affectation_cellule();
    test_voisins();
    test_conservation_qm();
    test_ajout_particules();
    std::cout << "\nTous les tests Univers sont passes.\n";
    return 0;
}