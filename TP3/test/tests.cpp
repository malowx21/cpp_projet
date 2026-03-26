#include <gtest/gtest.h>
#include "Vecteur.hpp"
#include "Particule.hpp"
#include "Univers.hpp"
#include <cmath>

//tests de la classe Vecteur

// ./tests        # lance tous les tests
// ./tests --gtest_filter=VecteurTest.*   # seulement Vecteur
TEST(VecteurTest, ConstructeurDefaut) {
    Vecteur v;
    EXPECT_EQ(v.getX(), 0.0);
    EXPECT_EQ(v.getY(), 0.0);
    EXPECT_EQ(v.getZ(), 0.0);
}

TEST(VecteurTest, ConstructeurParametre) {
    Vecteur v(1.0, 2.0, 3.0);
    EXPECT_EQ(v.getX(), 1.0);
    EXPECT_EQ(v.getY(), 2.0);
    EXPECT_EQ(v.getZ(), 3.0);
}

TEST(VecteurTest, Norme) {
    Vecteur v(3.0, 4.0, 0.0);
    EXPECT_NEAR(v.norm(), 5.0, 1e-9);
}

TEST(VecteurTest, Addition) {
    Vecteur v1(1.0, 2.0, 3.0);
    Vecteur v2(4.0, 5.0, 6.0);
    Vecteur v3 = v1 + v2;
    EXPECT_EQ(v3.getX(), 5.0);
    EXPECT_EQ(v3.getY(), 7.0);
    EXPECT_EQ(v3.getZ(), 9.0);
}

TEST(VecteurTest, Soustraction) {
    Vecteur v1(4.0, 5.0, 6.0);
    Vecteur v2(1.0, 2.0, 3.0);
    Vecteur v3 = v1 - v2;
    EXPECT_EQ(v3.getX(), 3.0);
    EXPECT_EQ(v3.getY(), 3.0);
    EXPECT_EQ(v3.getZ(), 3.0);
}

TEST(VecteurTest, MultiplicationScalaire) {
    Vecteur v(1.0, 2.0, 3.0);
    Vecteur v2 = v * 2.0;
    EXPECT_EQ(v2.getX(), 2.0);
    EXPECT_EQ(v2.getY(), 4.0);
    EXPECT_EQ(v2.getZ(), 6.0);
}

TEST(VecteurTest, DivisionScalaire) {
    Vecteur v(2.0, 4.0, 6.0);
    Vecteur v2 = v / 2.0;
    EXPECT_NEAR(v2.getX(), 1.0, 1e-9);
    EXPECT_NEAR(v2.getY(), 2.0, 1e-9);
    EXPECT_NEAR(v2.getZ(), 3.0, 1e-9);
}

TEST(VecteurTest, PlusEgal) {
    Vecteur v(1.0, 0.0, 0.0);
    v += Vecteur(0.0, 1.0, 0.0);
    EXPECT_EQ(v.getX(), 1.0);
    EXPECT_EQ(v.getY(), 1.0);
    EXPECT_EQ(v.getZ(), 0.0);
}

TEST(VecteurTest, MoinsEgal) {
    Vecteur v(3.0, 3.0, 0.0);
    v -= Vecteur(1.0, 1.0, 0.0);
    EXPECT_EQ(v.getX(), 2.0);
    EXPECT_EQ(v.getY(), 2.0);
    EXPECT_EQ(v.getZ(), 0.0);
}

//tests de la classe Particule

TEST(ParticuleTest, ConstructeurDefaut) {
    Particule p;
    EXPECT_EQ(p.getMasse(), 0.0);
    EXPECT_EQ(p.getId(), 0);
}

TEST(ParticuleTest, Setters) {
    Particule p;
    p.setPosition(Vecteur(1.0, 2.0, 3.0));
    p.setVitesse(Vecteur(0.5, 0.5, 0.0));
    p.setMasse(2.0);
    p.setId(42);

    EXPECT_EQ(p.getPosition().getX(), 1.0);
    EXPECT_EQ(p.getVitesse().getX(), 0.5);
    EXPECT_EQ(p.getMasse(), 2.0);
    EXPECT_EQ(p.getId(), 42);
}

//tests de la classe Univers

TEST(UniversTest, AjouterParticule) {
    Univers U(3);
    Particule p;
    p.setMasse(1.0);
    p.setId(0);
    U.ajouterParticule(p);
    EXPECT_EQ(U.getParticules().size(), 1);
}

TEST(UniversTest, ConservationImpulsion) {
    // apres calcul des forces, la somme doit etre nulle (Newton 3)
    Univers U(3);
    for (int i = 0; i < 5; ++i) {
        Particule p;
        p.setPosition(Vecteur(i * 0.3, i * 0.1, 0.0));
        p.setMasse(1.0);
        p.setId(i);
        U.ajouterParticule(p);
    }
    U.calcul_forces();

    Vecteur somme(0, 0, 0);
    for (const auto& p : U.getParticules())
        somme += p.getForce();

    EXPECT_NEAR(somme.norm(), 0.0, 1e-6);
}

TEST(UniversTest, AvancerTemps) {
    // le temps doit augmenter apres avancer()
    Univers U(3);
    Particule p;
    p.setPosition(Vecteur(0.0, 0.0, 0.0));
    p.setVitesse(Vecteur(1.0, 0.0, 0.0));
    p.setMasse(1.0);
    p.setId(0);
    U.ajouterParticule(p);

    EXPECT_EQ(U.getTemps(), 0.0);
    U.avancer(0.1, 10.0);
    EXPECT_NEAR(U.getTemps(), 0.1, 1e-9);
}
