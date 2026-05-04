#include <gtest/gtest.h>
#include "Particule.hpp"
#include <cmath>


//  Tests : Ces tests vérifient que la classe Particule se comporte correctement via ses différentes méthodes.

// Vérification de l'initialisation d'une particule par le constructeur par défaut 
TEST(ParticuleTest, ConstructeurParDefaut) {
    Particule p;

    EXPECT_NEAR(p.getMasse(), 0.0, 1e-10);
    EXPECT_EQ(p.getId(), 0);
    // On test si les vecteurs sont nuls
    EXPECT_NEAR(p.getPosition().norm(), 0.0, 1e-10);
    EXPECT_NEAR(p.getVitesse().norm(),  0.0, 1e-10);
    EXPECT_NEAR(p.getForce().norm(),    0.0, 1e-10);
}

// Test des accesseurs et mutateurs 
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

// Vérification de la possibilité de  modifier de force via reférence 
TEST(ParticuleTest, ForceMutable) {
    Particule p;

    p.setMasse(1.0);
    // l'ajout d'un vecteur à la force existante sans passer par un setter
    p.getForce() += Vecteur(3, 0, 0);

    EXPECT_NEAR(p.getForce().getX(), 3.0, 1e-10);
}

// Cycle de vie d'une particule
TEST(ParticuleTest, CycleDeVie) {
    Particule p;

    p.setId(1);
    p.setMasse(1.0);
    p.setPosition(Vecteur(1,2,3));
    p.setVitesse(Vecteur(0,1,0));
    // Construction par recopie 
    Particule p_copy(p);
    // Modification de la copie 
    p_copy.setPosition(Vecteur(10,10,10));

    // Utilisation de l'opérateur d'affectation
    Particule p_assign;
    p_assign = p_copy;

    // Vérification des nouvelles assignations
    EXPECT_EQ(p_assign.getId(), 1);
    EXPECT_NEAR(p_assign.getPosition().getX(), 10.0, 1e-10);

    // On test si le cas de base est inchngé
    EXPECT_NEAR(p.getPosition().getX(), 1.0, 1e-10);
}

// Test de l'auto-affectation 
TEST(ParticuleTest, AutoAffectation) {
    Particule p;
    p.setId(42);
    p = p; 
    EXPECT_EQ(p.getId(), 42);
}

// Test de la surcharge de l'opérateur de flux
TEST(ParticuleTest, FluxSortie) {
    Particule p;
    p.setId(5);
    p.setMasse(2.0);
    std::ostringstream oss;
    oss << p;
    std::string output = oss.str();
    // On cherche si les mots clés apparaissent dans la chaîne de caractères
    EXPECT_NE(output.find("Particule"), std::string::npos);
    EXPECT_NE(output.find("5"), std::string::npos);
}

// Test de la modification via référence
TEST(ParticuleTest, ForceReferenceConsistence) {
    Particule p;
    Vecteur& f = p.getForce();
    f += Vecteur(5,0,0);

    EXPECT_NEAR(p.getForce().getX(), 5.0, 1e-10);
}