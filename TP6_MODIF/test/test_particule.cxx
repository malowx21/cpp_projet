#include <gtest/gtest.h>
#include "Particule.hpp"
#include <cmath>


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
//Cycle de vie
TEST(ParticuleTest, FullLifecycleFlow) {
    Particule p;

    
    p.setId(1);
    p.setMasse(1.0);
    p.setPosition(Vecteur(1,2,3));
    p.setVitesse(Vecteur(0,1,0));

   
    Particule p_copy(p);

   
    p_copy.setPosition(Vecteur(10,10,10));

    // Step 4: assign back
    Particule p_assign;
    p_assign = p_copy;

    // Checks
    EXPECT_EQ(p_assign.getId(), 1);
    EXPECT_NEAR(p_assign.getPosition().getX(), 10.0, 1e-10);

    // original unchanged
    EXPECT_NEAR(p.getPosition().getX(), 1.0, 1e-10);
}

TEST(ParticuleTest, SelfAssignment) {
    Particule p;
    p.setId(42);

    p = p; 

    EXPECT_EQ(p.getId(), 42);
}

TEST(ParticuleTest, StreamOutputFormat) {
    Particule p;
    p.setId(5);
    p.setMasse(2.0);

    std::ostringstream oss;
    oss << p;

    std::string output = oss.str();

    EXPECT_NE(output.find("Particule"), std::string::npos);
    EXPECT_NE(output.find("5"), std::string::npos);
}
TEST(ParticuleTest, ForceReferenceConsistency) {
    Particule p;

    Vecteur& f = p.getForce();
    f += Vecteur(5,0,0);

    EXPECT_NEAR(p.getForce().getX(), 5.0, 1e-10);
}