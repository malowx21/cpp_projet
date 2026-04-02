#include "Univers.hpp"
#include <cmath>
#include <iostream>

static bool approx(double a, double b, double tol = 1e-8) {
    return std::abs(a - b) < tol;
}

static void check(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "FAIL : " << msg << "\n";
        std::exit(1);
    }
    std::cout << "  OK : " << msg << "\n";
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


void test_distance_equilibre() {
    std::cout << "[ForcesLJ] zero de la force a r_eq = 2^(1/6)*sigma\n";

    const double eps = 1.0, sig = 1.0;
    double r_eq = std::pow(2.0, 1.0 / 6.0) * sig;
    double F    = force_lj_analytique(eps, sig, r_eq);
    check(approx(F, 0.0, 1e-12), "F(r_eq) == 0");
}


void test_signe_force() {
    std::cout << "[ForcesLJ] signe de la force\n";

    const double eps = 1.0, sig = 1.0;
    double r_eq = std::pow(2.0, 1.0 / 6.0) * sig;

    // r < r_eq : facteur < 0 (force répulsive, s'éloigne de j)
    double F_rep = force_lj_analytique(eps, sig, r_eq * 0.95);
    check(F_rep < 0, "F < 0 (repulsive, facteur negatif) pour r < r_eq");

    // r > r_eq : facteur > 0 (force attractive, se rapproche de j)
    double F_att = force_lj_analytique(eps, sig, r_eq * 1.05);
    check(F_att > 0, "F > 0 (attractive, facteur positif) pour r > r_eq");
}


void test_newton3() {
    std::cout << "[ForcesLJ] Newton 3 (F_ij == -F_ji)\n";

    const double eps = 1.0, sig = 1.0, rcut = 2.5;
    Univers U(2, eps, sig, rcut, {20.0, 20.0, 1.0});

    // Distance 1.2*sigma, bien dans le rayon de coupure
    U.ajouter_particule(make_part(0, 9.4, 10.0, 0.0));
    U.ajouter_particule(make_part(1, 10.6, 10.0, 0.0));
    U.mettre_a_jour_cellules();
    U.calculer_forces_lj();

    const auto& ps = U.get_particules();
    double fx0 = ps[0].getForce().getX();
    double fx1 = ps[1].getForce().getX();

    check(approx(fx0, -fx1, 1e-10), "Fx0 == -Fx1  (Newton 3 axe x)");
    check(approx(ps[0].getForce().getY(), -ps[1].getForce().getY(), 1e-10),
          "Fy0 == -Fy1  (Newton 3 axe y)");
}


void test_rcut() {
    std::cout << "[ForcesLJ] force nulle au-dela de rcut\n";

    const double eps = 1.0, sig = 1.0, rcut = 2.5;
    Univers U(2, eps, sig, rcut, {20.0, 20.0, 1.0});

    // Distance = 3.0 > rcut
    U.ajouter_particule(make_part(0,  8.5, 10.0, 0.0));
    U.ajouter_particule(make_part(1, 11.5, 10.0, 0.0));
    U.mettre_a_jour_cellules();
    U.calculer_forces_lj();

    const auto& ps = U.get_particules();
    check(approx(ps[0].getForce().norm(), 0.0), "force nulle sur p0 (r > rcut)");
    check(approx(ps[1].getForce().norm(), 0.0), "force nulle sur p1 (r > rcut)");
}


void test_valeur_analytique() {
    std::cout << "[ForcesLJ] valeur analytique de la force\n";

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

    check(approx(Fx_sim, Fx_anal, 1e-8),
          "Fx simulee == Fx analytique pour r=1.1*sigma, eps=2");
}


int main() {
    test_distance_equilibre();
    test_signe_force();
    test_newton3();
    test_rcut();
    test_valeur_analytique();
    std::cout << "\nTous les tests Forces LJ sont passes.\n";
    return 0;
}