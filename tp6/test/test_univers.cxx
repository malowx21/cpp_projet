#include <gtest/gtest.h>
#include "Univers.hpp"
#include <cmath>
#include <vector>

//  Création d'une particule au repos 
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

//  Tests : ces tests vérifient le comportement global de la class Univers 

// Test du découpage de l'univers en grille de cellule 
TEST(UniversTest, GrilleDimensions) {
    // domaine 10x10x1 et  rcut=2.5 donc 16 cellules
    Univers U(2, 1.0, 1.0, 2.5, {10.0, 10.0, 1.0});
    EXPECT_EQ(U.get_dimension(), 2);
    // Vérification via get_cellule_index sur les coins
    int idx_origine = U.get_cellule_index(Vecteur(0.1, 0.1, 0.0));
    EXPECT_EQ(idx_origine, 0);
    // Vérifivation de l'indexation de la dernière cellule 
    int idx_max = U.get_cellule_index(Vecteur(9.9, 9.9, 0.0));
    EXPECT_EQ(idx_max, 15); 
}

// Calcule de l'index d'une cellule selon la position d'une particule 
TEST(UniversTest, AffectationCellule) {
    // domaine 10x10x1, rcut=2.5 => taille_cellule = 2.5
    Univers U(2, 1.0, 1.0, 2.5, {10.0, 10.0, 1.0});

    // Test manuel du calcul d'index (i + j * nb_cell_x)
    EXPECT_EQ(U.get_cellule_index(Vecteur(1.0, 1.0, 0.0)), 0);
    EXPECT_EQ(U.get_cellule_index(Vecteur(6.0, 7.5, 0.0)), 14);

    // Après ajout et mise à jour, la cellule 0 doit être dans ses propres voisins
    U.ajouter_particule(make_particule(0, 1.0, 1.0, 0.0));
    U.mettre_a_jour_cellules();

    auto voisins = U.get_voisins(0);
    bool contient_0 = false;
    for (int v : voisins) if (v == 0) contient_0 = true;

    EXPECT_TRUE(contient_0);
}

// Test des voisins de cellule
TEST(UniversTest, Voisins) {
    // grille 3x3x1 donc  taille_cellule = 3
    Univers U(2, 1.0, 1.0, 3.0, {9.0, 9.0, 1.0});

    // Cellule centrale (1,1,0) donc son index  est 4 : 9 voisins
    auto v_centre = U.get_voisins(4);
    EXPECT_EQ((int)v_centre.size(), 9);

    // Coin (0,0,0) donc son index est 0 : 4 voisins en deux dimensions
    auto v_coin = U.get_voisins(0);
    EXPECT_EQ((int)v_coin.size(), 4);

    // Bord milieu (1,0,0) donc son index est  1 : 6 voisins en deux dimensions
    auto v_bord = U.get_voisins(1);
    EXPECT_EQ((int)v_bord.size(), 6);
}

// Test de la conservation de la quantite de mouvement
TEST(UniversTest, ConservationQuantiteMouvement) {
    Univers U(2, 1.0, 1.0, 2.5, {20.0, 20.0, 1.0});

    // Deux particules avec des vitesses opposées 
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

// Verification que l'ajout de particules incrémente correctement la taille du conteneur 
TEST(UniversTest, AjoutParticules) {
    Univers U(2, 1.0, 1.0, 2.5, {20.0, 20.0, 1.0});

    EXPECT_TRUE(U.get_particules().empty());

    U.ajouter_particule(make_particule(0, 5.0, 5.0, 0.0));
    U.ajouter_particule(make_particule(1, 10.0, 10.0, 0.0));

    EXPECT_EQ((int)U.get_particules().size(), 2);
}


TEST(WallsTest, ReflexionX) {
    Univers U(2, 1.0, 1.0, 2.5, {10,10,1}, 0);

    Particule p;
    p.setPosition(Vecteur(-1, 5, 0));
    p.setVitesse(Vecteur(-2, 0, 0));
    p.setMasse(1);

    U.ajouter_particule(p);

    U.appliquer_conditions_limites();

    auto& P = U.get_particules()[0];

    EXPECT_GT(P.getPosition().getX(), 0);
    EXPECT_GT(P.getVitesse().getX(), 0); // inverted
}

// Tests concernant les conditions limites du tp6

// Vérification la périodicité des parois 
TEST(WallsTest, PeriodiqueX) {
    Univers U(2, 1.0, 1.0, 2.5, {10,10,1}, 1);

    Particule p;
    p.setPosition(Vecteur(11, 5, 0));
    p.setVitesse(Vecteur(1, 0, 0));
    p.setMasse(1);

    U.ajouter_particule(p);
    U.appliquer_conditions_limites();
    auto& P = U.get_particules()[0];

    EXPECT_LT(P.getPosition().getX(), 10);
}

// Vérification que les parois sont absorbantes 
TEST(WallsTest, Absorption) {
    Univers U(2, 1.0, 1.0, 2.5, {10,10,1}, 2);

    Particule p;
    p.setPosition(Vecteur(-1, 0, 0));
    p.setMasse(1);

    U.ajouter_particule(p);

    U.appliquer_conditions_limites();

    EXPECT_TRUE(U.get_particules().empty());
}

// Vérification de la mise en echelle des vitesses , notamment l'énérgie cinétique qui doit etre égale à E_cible
TEST(PhysicsTest, RescalingWorks) {
    Univers U(2);

    Particule p;
    p.setPosition(Vecteur(5,5,0));
    p.setVitesse(Vecteur(10,0,0));
    p.setMasse(1);

    U.ajouter_particule(p);
    //double Ec_before = U.energie_cinetique();
    U.rescaler_vitesses(1.0);
    double Ec_after = U.energie_cinetique();

    EXPECT_NEAR(Ec_after, 1.0, 1e-6);
}
