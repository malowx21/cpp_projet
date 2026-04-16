#include "Particule.hpp"
#include <cmath>
#include <iostream>

static bool approx(double a, double b, double tol = 1e-10) {
    return std::abs(a - b) < tol;
}

static void check(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "FAIL : " << msg << "\n";
        std::exit(1);
    }
    std::cout << "  OK : " << msg << "\n";
}


void test_defaut() {
    std::cout << "[Particule] constructeur par defaut\n";

    Particule p;
    check(approx(p.getMasse(), 0.0), "masse initiale == 0");
    check(p.getId() == 0,            "id initial == 0");
    check(approx(p.getPosition().norm(), 0.0), "position initiale == (0,0,0)");
    check(approx(p.getVitesse().norm(),  0.0), "vitesse initiale == (0,0,0)");
    check(approx(p.getForce().norm(),    0.0), "force initiale == (0,0,0)");
}

void test_setters_getters() {
    std::cout << "[Particule] setters / getters\n";

    Particule p;
    p.setId(42);
    p.setMasse(2.5);
    p.setPosition(Vecteur(1, 2, 3));
    p.setVitesse (Vecteur(0, -1, 0));
    p.setForce   (Vecteur(0,  0, 9.81));

    check(p.getId() == 42,           "setId / getId");
    check(approx(p.getMasse(), 2.5), "setMasse / getMasse");

    check(approx(p.getPosition().getX(), 1.0) &&
          approx(p.getPosition().getY(), 2.0) &&
          approx(p.getPosition().getZ(), 3.0), "setPosition / getPosition");

    check(approx(p.getVitesse().getY(), -1.0), "setVitesse / getVitesse");
    check(approx(p.getForce().getZ(), 9.81),   "setForce / getForce");
}

void test_force_mutable() {
    std::cout << "[Particule] modification de force via ref non-const\n";

    Particule p;
    p.setMasse(1.0);
    p.getForce() += Vecteur(3, 0, 0);
    check(approx(p.getForce().getX(), 3.0), "getForce() non-const += fonctionne");
}


int main() {
    test_defaut();
    test_setters_getters();
    test_force_mutable();
    std::cout << "\nTous les tests Particule sont passes.\n";
    return 0;
}