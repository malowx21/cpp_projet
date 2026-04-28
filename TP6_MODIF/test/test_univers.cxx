#include <gtest/gtest.h>
#include "Univers.hpp"
#include <cmath>
#include <vector>

//  UTILITAIRE 

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

//  TESTS 

// [Univers] dimensions de la grille
TEST(UniversTest, GrilleDimensions) {
    // domaine 10x10x1, rcut=2.5 => 4x4x1 = 16 cellules
    Univers U(2, 1.0, 1.0, 2.5, {10.0, 10.0, 1.0});

    EXPECT_EQ(U.get_dimension(), 2);

    // Vérification via get_cellule_index sur les coins
    int idx_origine = U.get_cellule_index(Vecteur(0.1, 0.1, 0.0));
    EXPECT_EQ(idx_origine, 0);

    int idx_max = U.get_cellule_index(Vecteur(9.9, 9.9, 0.0));
    EXPECT_EQ(idx_max, 15); // (4x4-1)
}

// [Univers] affectation cellule
TEST(UniversTest, AffectationCellule) {
    // domaine 10x10x1, rcut=2.5 => taille_cellule = 2.5
    Univers U(2, 1.0, 1.0, 2.5, {10.0, 10.0, 1.0});

    // (1.0, 1.0) => i=0, j=0 => index 0
    EXPECT_EQ(U.get_cellule_index(Vecteur(1.0, 1.0, 0.0)), 0);

    // (6.0, 7.5) => i=2, j=3 => index = 2 + 3*4 = 14
    EXPECT_EQ(U.get_cellule_index(Vecteur(6.0, 7.5, 0.0)), 14);

    // Après ajout + mise à jour, la cellule 0 doit être dans ses propres voisins
    U.ajouter_particule(make_particule(0, 1.0, 1.0, 0.0));
    U.mettre_a_jour_cellules();

    auto voisins = U.get_voisins(0);
    bool contient_0 = false;
    for (int v : voisins) if (v == 0) contient_0 = true;

    EXPECT_TRUE(contient_0);
}

// [Univers] voisins de cellules
TEST(UniversTest, Voisins) {
    // grille 3x3x1 => taille_cellule = 3.0
    Univers U(2, 1.0, 1.0, 3.0, {9.0, 9.0, 1.0});

    // Cellule centrale (1,1,0) -> index 4 : 9 voisins
    auto v_centre = U.get_voisins(4);
    EXPECT_EQ((int)v_centre.size(), 9);

    // Coin (0,0,0) -> index 0 : 4 voisins en 2D
    auto v_coin = U.get_voisins(0);
    EXPECT_EQ((int)v_coin.size(), 4);

    // Bord milieu (1,0,0) -> index 1 : 6 voisins en 2D
    auto v_bord = U.get_voisins(1);
    EXPECT_EQ((int)v_bord.size(), 6);
}

// [Univers] conservation de la quantite de mouvement
TEST(UniversTest, ConservationQuantiteMouvement) {
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

    EXPECT_NEAR(qm_init, qm_x(), 1e-8);
}

// [Univers] ajout de particules
TEST(UniversTest, AjoutParticules) {
    Univers U(2, 1.0, 1.0, 2.5, {20.0, 20.0, 1.0});

    EXPECT_TRUE(U.get_particules().empty());

    U.ajouter_particule(make_particule(0, 5.0, 5.0, 0.0));
    U.ajouter_particule(make_particule(1, 10.0, 10.0, 0.0));

    EXPECT_EQ((int)U.get_particules().size(), 2);
}