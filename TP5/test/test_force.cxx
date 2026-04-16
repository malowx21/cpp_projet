#include <gtest/gtest.h>
#include "Univers.hpp"
#include <cmath>

//  UTILITAIRES 

static bool approx(double a, double b, double tol = 1e-8) {
    return std::abs(a - b) < tol;
}

static Particule make_part(int id, double x, double y, double z,
                           double masse = 1.0) {
    Particule p;
    p.setId(id);
    p.setMasse(masse);
    p.setPosition(Vecteur(x, y, z));
    p.setVitesse(Vecteur(0, 0, 0));
    p.setForce(Vecteur(0, 0, 0));
    return p;
}

static double force_lj_analytique(double eps, double sig, double r) {
    double sr2  = (sig * sig) / (r * r);
    double sr6  = sr2 * sr2 * sr2;
    double sr12 = sr6 * sr6;
    return 24.0 * eps / (r * r) * (sr6 - 2.0 * sr12) * r;
}

//  TESTS 

// [ForcesLJ] zero de la force a r_eq = 2^(1/6)*sigma
TEST(ForcesLJTest, DistanceEquilibre) {
    const double eps = 1.0, sig = 1.0;
    double r_eq = std::pow(2.0, 1.0 / 6.0) * sig;
    double F    = force_lj_analytique(eps, sig, r_eq);

    EXPECT_NEAR(F, 0.0, 1e-12);
}

// [ForcesLJ] signe de la force
TEST(ForcesLJTest, SigneForce) {
    const double eps = 1.0, sig = 1.0;
    double r_eq = std::pow(2.0, 1.0 / 6.0) * sig;

    // r < r_eq : facteur < 0 (force répulsive, s'éloigne de j)
    double F_rep = force_lj_analytique(eps, sig, r_eq * 0.95);
    EXPECT_LT(F_rep, 0);

    // r > r_eq : facteur > 0 (force attractive, se rapproche de j)
    double F_att = force_lj_analytique(eps, sig, r_eq * 1.05);
    EXPECT_GT(F_att, 0);
}

// [ForcesLJ] Newton 3 (F_ij == -F_ji)
TEST(ForcesLJTest, Newton3) {
    const double eps = 1.0, sig = 1.0, rcut = 2.5;
    Univers U(2, eps, sig, rcut, {20.0, 20.0, 1.0});

    // Distance 1.2*sigma, bien dans le rayon de coupure
    U.ajouter_particule(make_part(0, 9.4, 10.0, 0.0));
    U.ajouter_particule(make_part(1, 10.6, 10.0, 0.0));

    U.mettre_a_jour_cellules();
    U.calculer_forces_lj();

    const auto& ps = U.get_particules();

    EXPECT_NEAR(ps[0].getForce().getX(),
                -ps[1].getForce().getX(), 1e-10);

    EXPECT_NEAR(ps[0].getForce().getY(),
                -ps[1].getForce().getY(), 1e-10);
}

// [ForcesLJ] force nulle au-dela de rcut
TEST(ForcesLJTest, Rcut) {
    const double eps = 1.0, sig = 1.0, rcut = 2.5;
    Univers U(2, eps, sig, rcut, {20.0, 20.0, 1.0});

    // Distance = 3.0 > rcut
    U.ajouter_particule(make_part(0,  8.5, 10.0, 0.0));
    U.ajouter_particule(make_part(1, 11.5, 10.0, 0.0));

    U.mettre_a_jour_cellules();
    U.calculer_forces_lj();

    const auto& ps = U.get_particules();

    EXPECT_NEAR(ps[0].getForce().norm(), 0.0, 1e-10);
    EXPECT_NEAR(ps[1].getForce().norm(), 0.0, 1e-10);
}

// [ForcesLJ] valeur analytique de la force
TEST(ForcesLJTest, ValeurAnalytique) {
    const double eps = 2.0, sig = 1.0, rcut = 5.0;
    const double r   = 1.1;

    Univers U(2, eps, sig, rcut, {20.0, 20.0, 1.0});

    U.ajouter_particule(make_part(0, 10.0,     10.0, 0.0));
    U.ajouter_particule(make_part(1, 10.0 + r, 10.0, 0.0));

    U.mettre_a_jour_cellules();
    U.calculer_forces_lj();

    const auto& ps = U.get_particules();

    double Fx_sim  = ps[0].getForce().getX();
    double Fx_anal = force_lj_analytique(eps, sig, r);

    EXPECT_NEAR(Fx_sim, Fx_anal, 1e-8);
}