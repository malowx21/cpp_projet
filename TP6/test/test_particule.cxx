#include <gtest/gtest.h>
#include "Particule.hpp"
#include <cmath>

//  UTILITAIRE 

static bool approx(double a, double b, double tol = 1e-10) {
    return std::abs(a - b) < tol;
}

//  TESTS 

// [Particule] constructeur par defaut
TEST(ParticuleTest, ConstructeurParDefaut) {
    Particule p;

    EXPECT_NEAR(p.getMasse(), 0.0, 1e-10);
    EXPECT_EQ(p.getId(), 0);

    EXPECT_NEAR(p.getPosition().norm(), 0.0, 1e-10);
    EXPECT_NEAR(p.getVitesse().norm(),  0.0, 1e-10);
    EXPECT_NEAR(p.getForce().norm(),    0.0, 1e-10);
}

// [Particule] setters / getters
TEST(ParticuleTest, SettersGetters) {
    Particule p;

    p.setId(42);
    p.setMasse(2.5);
    p.setPosition(Vecteur(1, 2, 3));
    p.setVitesse (Vecteur(0, -1, 0));
    p.setForce   (Vecteur(0,  0, 9.81));

    EXPECT_EQ(p.getId(), 42);
    EXPECT_NEAR(p.getMasse(), 2.5, 1e-10);

    EXPECT_NEAR(p.getPosition().getX(), 1.0, 1e-10);
    EXPECT_NEAR(p.getPosition().getY(), 2.0, 1e-10);
    EXPECT_NEAR(p.getPosition().getZ(), 3.0, 1e-10);

    EXPECT_NEAR(p.getVitesse().getY(), -1.0, 1e-10);
    EXPECT_NEAR(p.getForce().getZ(), 9.81, 1e-10);
}

// [Particule] modification de force via ref non-const
TEST(ParticuleTest, ForceMutable) {
    Particule p;

    p.setMasse(1.0);
    p.getForce() += Vecteur(3, 0, 0);

    EXPECT_NEAR(p.getForce().getX(), 3.0, 1e-10);
}