#include "Vecteur.hpp"
#include <cmath>
#include <cassert>
#include <iostream>

// ----------------------------------------------------------------
//  Helpers
// ----------------------------------------------------------------
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

// ----------------------------------------------------------------
//  Tests
// ----------------------------------------------------------------
void test_constructeurs() {
    std::cout << "[Vecteur] constructeurs\n";

    Vecteur v0;
    check(v0.getX() == 0 && v0.getY() == 0 && v0.getZ() == 0,
          "constructeur par defaut -> (0,0,0)");

    Vecteur v(1.0, -2.5, 3.0);
    check(v.getX() == 1.0 && v.getY() == -2.5 && v.getZ() == 3.0,
          "constructeur (x,y,z)");
}

void test_norme() {
    std::cout << "[Vecteur] norme\n";

    check(approx(Vecteur(3, 4, 0).norm(), 5.0),
          "norm(3,4,0) == 5");
    check(approx(Vecteur(0, 0, 0).norm(), 0.0),
          "norm(0,0,0) == 0");
    check(approx(Vecteur(1, 1, 1).norm(), std::sqrt(3.0)),
          "norm(1,1,1) == sqrt(3)");
}

void test_addition() {
    std::cout << "[Vecteur] addition\n";

    Vecteur a(1, 2, 3), b(4, -1, 0);
    Vecteur c = a + b;
    check(approx(c.getX(), 5) && approx(c.getY(), 1) && approx(c.getZ(), 3),
          "(1,2,3) + (4,-1,0) == (5,1,3)");

    a += b;
    check(approx(a.getX(), 5) && approx(a.getY(), 1) && approx(a.getZ(), 3),
          "operator+= correct");
}

void test_soustraction() {
    std::cout << "[Vecteur] soustraction\n";

    Vecteur a(5, 3, 1), b(2, 4, -1);
    Vecteur c = a - b;
    check(approx(c.getX(), 3) && approx(c.getY(), -1) && approx(c.getZ(), 2),
          "(5,3,1) - (2,4,-1) == (3,-1,2)");
}

void test_scalaire() {
    std::cout << "[Vecteur] multiplication / division scalaire\n";

    Vecteur v(2, -4, 6);
    Vecteur m = v * 0.5;
    check(approx(m.getX(), 1) && approx(m.getY(), -2) && approx(m.getZ(), 3),
          "(2,-4,6) * 0.5 == (1,-2,3)");

    Vecteur d = v / 2.0;
    check(approx(d.getX(), 1) && approx(d.getY(), -2) && approx(d.getZ(), 3),
          "(2,-4,6) / 2 == (1,-2,3)");
}

// ----------------------------------------------------------------
int main() {
    test_constructeurs();
    test_norme();
    test_addition();
    test_soustraction();
    test_scalaire();
    std::cout << "\nTous les tests Vecteur sont passes.\n";
    return 0;
}