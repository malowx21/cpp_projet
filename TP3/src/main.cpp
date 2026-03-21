#include "Vecteur.hpp"
#include "Particule.hpp"
#include "Univers.hpp"

#include <iostream>
#include <chrono>
#include <cmath>
#include <cassert>
#include <vector>
#include <random>


//Q3 -Tests unitaires de la classe Vecteur

void test_vecteur() {
    std::cout << "Tests Vecteur :" << std::endl;

    // Constructeurs
    Vecteur v0;
    assert(v0.x == 0 && v0.y == 0 && v0.z == 0);

    Vecteur v1(1.0, 2.0, 3.0);
    assert(v1.x == 1.0 && v1.y == 2.0 && v1.z == 3.0);

    // norm()
    Vecteur v2(3.0, 4.0, 0.0);
    assert(std::abs(v2.norm() - 5.0) < 1e-9);

    // operator+
    Vecteur v3 = v1 + v2;
    assert(v3.x == 4.0 && v3.y == 6.0 && v3.z == 3.0);

    // operator-
    Vecteur v4 = v1 - v2;
    assert(v4.x == -2.0 && v4.y == -2.0 && v4.z == 3.0);

    // operator* (scalaire)
    Vecteur v5 = v1 * 2.0;
    assert(v5.x == 2.0 && v5.y == 4.0 && v5.z == 6.0);

    // operator/
    Vecteur v6 = v1 / 2.0;
    assert(std::abs(v6.x - 0.5) < 1e-9 && std::abs(v6.y - 1.0) < 1e-9);

    // operator+=
    Vecteur v7(1.0, 0.0, 0.0);
    v7 += Vecteur(0.0, 1.0, 0.0);
    assert(v7.x == 1.0 && v7.y == 1.0 && v7.z == 0.0);

    std::cout << "  Tous les tests Vecteur passes." << std::endl;
}


//  Helpers


// Cree un univers avec N = (2^k)^3 particules, uniformement
// distribuees sur le cube [0;1]^3, vitesse initiale uniforme nulle.
Univers creer_univers(int k) {
    Univers U(3);
    int n = 1 << k;           // 2^k par dimension
    double step = 1.0 / n;

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist(0.0, step);

    int id = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int kk = 0; kk < n; ++kk) {
                Particule p;
                // position aleatoire dans la sous-cellule pour eviter
                // les superpositions exactes
                p.position = Vecteur(i *step + dist(rng),
                                     j *step + dist(rng),
                                     kk * step + dist(rng));
                p.vitesse  = Vecteur(0.0, 0.0, 0.0);
                p.force    = Vecteur(0.0, 0.0, 0.0);
                p.m        = 1.0;
                p.id       = id++;
                U.particules.push_back(p);
            }
    return U;
}


//Q6 -Univers avec (2^5)^3 particules (k=5)

void demo_univers_k5() {
    std::cout << "\n Q6 : creation univers k=5 " << std::endl;
    Univers U = creer_univers(5);
    std::cout << "  Nombre de particules : " << U.particules.size()
              << " (attendu " << (1 << 5) * (1 << 5) * (1 << 5) << ")" << std::endl;
}


//Q7 - Benchmark insertion

void benchmark_insertion() {
    std::cout << "\n Q7 : benchmark insertion: " << std::endl;

    for (int k = 3; k <= 7; ++k) {
        auto t0 = std::chrono::high_resolution_clock::now();
        Univers U = creer_univers(k);
        auto t1 = std::chrono::high_resolution_clock::now();

        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::cout << "  k=" << k
                  << "  N=" << U.particules.size()
                  << "  temps insertion : " << ms << " ms" << std::endl;
    }
}


//Q8 - Benchmark calcul des interactions

void benchmark_forces() {
    std::cout << "\n Q8 : benchmark calcul_forces: " << std::endl;

    for (int k = 3; k <= 7; ++k) {
        Univers U = creer_univers(k);
        int N = U.particules.size();

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


//  Q9 – Verification que Newton 3 est bien applique
//  (la symetrie est deja dans Univers::calcul_forces via j=i+1)
//  On verifie ici que la somme des forces est nulle (conservation)

void verif_conservation_impulsion() {
    std::cout << "\n Q9 : verification conservation impulsion: " << std::endl;

    Univers U = creer_univers(3);
    U.calcul_forces();

    Vecteur somme(0, 0, 0);
    for (const auto& p : U.particules)
        somme += p.force;

    std::cout << "  Somme des forces : ("
              << somme.x << ", " << somme.y << ", " << somme.z << ")"
              << "  |somme| = " << somme.norm() << std::endl;

    assert(somme.norm() < 1e-6 && "La 3eme loi de Newton n'est pas respectee !");
    std::cout << "  Conservation de l'impulsion verifiee (|somme| < 1e-6)." << std::endl;
}


//  Q10 – Simplifications possibles (demonstration commentee)
//
//  1. Symetrie Newton 3 ce qui est deja fait, divise le nb de paires par 2
//  2. Parallelisme OpenMP : #pragma omp parallel for reduction(...)
//     sur la boucle externe -- facile a ajouter, gain ~nb_coeurs
//  3. Hierarchie spatiale (Barnes-Hut, octree) : O(N log N) au lieu
//     de O(N^2). Pour N > 10^4 cela devient indispensable.
//  4. Precalcul de 1/dist^3 pour eviter la double division.
//  5. Structures de donnees cache-friendly (SoA au lieu de AoS).
//
//  La fonction ci-dessous illustre le point 4 (deja present dans
//  calcul_forces) et mesure le gain du point 1.

void demo_simplifications() {
    std::cout << "\n Q10 : comparaison naive vs Newton-3 :" << std::endl;

    int k = 5;
    Univers U = creer_univers(k);
    int N = U.particules.size();

    // Version naive : boucle complete O(N^2) sans Newton 3
    auto t0 = std::chrono::high_resolution_clock::now();
    {
        double G = 1.0, eps = 0.05;
        for (auto& p : U.particules) p.force = Vecteur(0,0,0);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                if (i == j) continue;
                Vecteur rij = U.particules[j].position - U.particules[i].position;
                double dist = rij.norm();
                double factor = G * U.particules[i].m * U.particules[j].m
                                / (dist*dist*dist + eps*eps);
                U.particules[i].force += rij * factor;
            }
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    double ms_naive = std::chrono::duration<double, std::milli>(t1 - t0).count();

    // Version Newton 3 (deja implementee)
    auto t2 = std::chrono::high_resolution_clock::now();
    U.calcul_forces();
    auto t3 = std::chrono::high_resolution_clock::now();
    double ms_newton3 = std::chrono::duration<double, std::milli>(t3 - t2).count();

    std::cout << "  N=" << N << std::endl;
    std::cout << "  Naive    : " << ms_naive   << " ms" << std::endl;
    std::cout << "  Newton 3 : " << ms_newton3 << " ms" << std::endl;
    std::cout << "  Ratio    : " << ms_naive / ms_newton3 << "x" << std::endl;
}


//  main

int main() {
    //Q3 – tests unitaires
    test_vecteur();

    //Q6 – creation de l'univers reference
    demo_univers_k5();

    //Q7 – benchmark insertion
    benchmark_insertion();

    //Q8 – benchmark calcul des forces, ça prend des jours  pour la derniere, commentez pour eviter le cout
    benchmark_forces();

    //Q9 – verification conservation impulsion (Newton 3) 
    verif_conservation_impulsion();

    //Q10 – comparaison naive vs optimise
    demo_simplifications();

    std::cout << "\nTous les tests et benchmarks termines." << std::endl;
    return 0;
}