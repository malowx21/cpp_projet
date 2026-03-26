#include "Vecteur.hpp"
#include "Particule.hpp"
#include "Univers.hpp"
#include <iostream>
#include <chrono>
#include <cmath>
#include <cassert>
#include <vector>
#include <random>

//tests unitaires de la classe Vecteur
void test_vecteur() {
    std::cout << "Tests Vecteur :" << std::endl;

    // constructeurs
    Vecteur v0;
    assert(v0.getX() == 0 && v0.getY() == 0 && v0.getZ() == 0);

    Vecteur v1(1.0, 2.0, 3.0);
    assert(v1.getX() == 1.0 && v1.getY() == 2.0 && v1.getZ() == 3.0);

    // norm()
    Vecteur v2(3.0, 4.0, 0.0);
    assert(std::abs(v2.norm() - 5.0) < 1e-9);

    // operator+
    Vecteur v3 = v1 + v2;
    assert(v3.getX() == 4.0 && v3.getY() == 6.0 && v3.getZ() == 3.0);

    // operator-
    Vecteur v4 = v1 - v2;
    assert(v4.getX() == -2.0 && v4.getY() == -2.0 && v4.getZ() == 3.0);

    // operator* scalaire
    Vecteur v5 = v1 * 2.0;
    assert(v5.getX() == 2.0 && v5.getY() == 4.0 && v5.getZ() == 6.0);

    // operator/
    Vecteur v6 = v1 / 2.0;
    assert(std::abs(v6.getX() - 0.5) < 1e-9 && std::abs(v6.getY() - 1.0) < 1e-9);

    // operator+=
    Vecteur v7(1.0, 0.0, 0.0);
    v7 += Vecteur(0.0, 1.0, 0.0);
    assert(v7.getX() == 1.0 && v7.getY() == 1.0 && v7.getZ() == 0.0);

    // operator-=
    Vecteur v8(3.0, 3.0, 0.0);
    v8 -= Vecteur(1.0, 1.0, 0.0);
    assert(v8.getX() == 2.0 && v8.getY() == 2.0 && v8.getZ() == 0.0);

    std::cout << "  Tous les tests Vecteur passes." << std::endl;
}

// cree un univers avec N = (2^k)^3 particules, uniformement
// distribuees sur le cube [0;1]^3, vitesse initiale nulle
Univers creer_univers(int k) {
    Univers U(3);
    int n = 1 << k;
    double step = 1.0 / n;

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, step);
    
    int id = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int kk = 0; kk < n; ++kk) {
                Particule p;
                p.setPosition(Vecteur(i * step + dist(rng),
                                      j * step + dist(rng),
                                      kk * step + dist(rng)));
                p.setVitesse(Vecteur(0.0, 0.0, 0.0));
                p.setForce(Vecteur(0.0, 0.0, 0.0));
                p.setMasse(1.0);
                p.setId(id++);
                U.ajouterParticule(p);
            }
    return U;
}

// univers avec (2^5)^3 particules k=5
void demo_univers_k5() {
    std::cout << "\n Q6 : creation univers k=5 " << std::endl;
    Univers U = creer_univers(5);
    std::cout << "  Nombre de particules : " << U.getParticules().size()
              << " (attendu " << (1 << 5) * (1 << 5) * (1 << 5) << ")" << std::endl;
}

//benchmark insertion
void benchmark_insertion() {
    std::cout << "\n Q7 : benchmark insertion: " << std::endl;

    for (int k = 3; k <= 7; ++k) {
        auto t0 = std::chrono::high_resolution_clock::now();
        Univers U = creer_univers(k);
        auto t1 = std::chrono::high_resolution_clock::now();

        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::cout << "  k=" << k
                  << "  N=" << U.getParticules().size()
                  << "  temps insertion : " << ms << " ms" << std::endl;
    }
}

//benchmark calcul des interactions
void benchmark_forces() {
    std::cout << "\n Q8 : benchmark calcul_forces: " << std::endl;

    for (int k = 3; k <= 5; ++k) {//on peut modifier ici pour 
        Univers U = creer_univers(k);
        int N = U.getParticules().size();

        auto t0 = std::chrono::high_resolution_clock::now();
        U.calcul_forces();
        auto t1 = std::chrono::high_resolution_clock::now();

        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::cout << "  k=" << k
                  << "  N=" << N
                  << "  temps calcul_forces : " << ms << " ms"
                  << "  (paires evaluees : " << (long long)N*(N-1)/2 << ")"
                  << std::endl;
    }
}

//verification conservation impulsion (Newton 3)
void verif_conservation_impulsion() {
    std::cout << "\n Q9 : verification conservation impulsion: " << std::endl;

    Univers U = creer_univers(3);
    U.calcul_forces();

    Vecteur somme(0, 0, 0);
    for (const auto& p : U.getParticules())
        somme += p.getForce();

    std::cout << "  Somme des forces : ("
              << somme.getX() << ", " << somme.getY() << ", " << somme.getZ() << ")"
              << "  |somme| = " << somme.norm() << std::endl;

    assert(somme.norm() < 1e-6 && "La 3eme loi de Newton n'est pas respectee !");
    std::cout << "  Conservation de l'impulsion verifiee (|somme| < 1e-6)." << std::endl;
}

//comparaison naive vs Newton-3
// simplifications possibles :
//- symetrie Newton 3, deja fait, divise le nb de paires par 2
//- parallelisme OpenMP sur la boucle externe, gain ~nb_coeurs
//- hierarchie spatiale Barnes-Hut octree : O(N log N) au lieu de O(N^2)
//- precalcul de 1/dist^3 pour eviter la double division
//- structures de donnees cache-friendly SoA au lieu de AoS
void demo_simplifications() {
    std::cout << "\n Q10 : comparaison naive vs Newton-3 :" << std::endl;

    int k = 5;
    Univers U = creer_univers(k);
    int N = U.getParticules().size();

    // version naive O(N^2) sans Newton 3
    auto t0 = std::chrono::high_resolution_clock::now();
    {
        double G = 1.0, eps = 0.05;
        for (auto& p : U.getParticules()) p.setForce(Vecteur(0, 0, 0));
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                if (i == j) continue;
                Vecteur rij = U.getParticules()[j].getPosition()
                            - U.getParticules()[i].getPosition();
                double dist = rij.norm();
                double factor = G * U.getParticules()[i].getMasse()
                              * U.getParticules()[j].getMasse()
                              / (dist*dist*dist + eps*eps);
                U.getParticules()[i].getForce() += rij * factor;
            }
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    double ms_naive = std::chrono::duration<double, std::milli>(t1 - t0).count();

    // version Newton 3 deja implementee
    auto t2 = std::chrono::high_resolution_clock::now();
    U.calcul_forces();
    auto t3 = std::chrono::high_resolution_clock::now();
    double ms_newton3 = std::chrono::duration<double, std::milli>(t3 - t2).count();

    std::cout << "  N=" << N << std::endl;
    std::cout << "  Naive    : " << ms_naive   << " ms" << std::endl;
    std::cout << "  Newton 3 : " << ms_newton3 << " ms" << std::endl;
    std::cout << "  Ratio    : " << ms_naive / ms_newton3 << "x" << std::endl;
}

// Simulation principale avec export VTK
int main() {
    //tests unitaires
    test_vecteur();

    

     //creation de l'univers reference
     demo_univers_k5();

     //benchmark insertion
     benchmark_insertion();
 
     // benchmark calcul des forces, k=3..5 pour eviter le temps trop long
     benchmark_forces();
 
     // verification conservation impulsion
     verif_conservation_impulsion();
 
     // comparaison naive vs optimise
     demo_simplifications();
    std::cout << "\n Simulation terminee" << std::endl;
    return 0;
}