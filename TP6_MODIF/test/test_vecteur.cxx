#include <gtest/gtest.h>
#include "Vecteur.hpp"
#include <cmath>


//  TESTS 
// [Vecteur] constructeurs
TEST(VecteurTest, Constructeurs) {
    Vecteur v0;
    EXPECT_EQ(v0.getX(), 0);
    EXPECT_EQ(v0.getY(), 0);
    EXPECT_EQ(v0.getZ(), 0);

    Vecteur v(1.0, -2.5, 3.0);
    EXPECT_DOUBLE_EQ(v.getX(), 1.0);
    EXPECT_DOUBLE_EQ(v.getY(), -2.5);
    EXPECT_DOUBLE_EQ(v.getZ(), 3.0);
}

// [Vecteur] norme
TEST(VecteurTest, Norme) {
    EXPECT_NEAR(Vecteur(3, 4, 0).norm(), 5.0, 1e-10);
    EXPECT_NEAR(Vecteur(0, 0, 0).norm(), 0.0, 1e-10);
    EXPECT_NEAR(Vecteur(1, 1, 1).norm(), std::sqrt(3.0), 1e-10);
}

// [Vecteur] addition
TEST(VecteurTest, Addition) {
    Vecteur a(1, 2, 3), b(4, -1, 0);
    Vecteur c = a + b;

    EXPECT_NEAR(c.getX(), 5, 1e-10);
    EXPECT_NEAR(c.getY(), 1, 1e-10);
    EXPECT_NEAR(c.getZ(), 3, 1e-10);

    a += b;
    EXPECT_NEAR(a.getX(), 5, 1e-10);
    EXPECT_NEAR(a.getY(), 1, 1e-10);
    EXPECT_NEAR(a.getZ(), 3, 1e-10);
}

// [Vecteur] soustraction
TEST(VecteurTest, Soustraction) {
    Vecteur a(5, 3, 1), b(2, 4, -1);
    Vecteur c = a - b;

    EXPECT_NEAR(c.getX(), 3, 1e-10);
    EXPECT_NEAR(c.getY(), -1, 1e-10);
    EXPECT_NEAR(c.getZ(), 2, 1e-10);
}

// [Vecteur] multiplication / division scalaire
TEST(VecteurTest, Scalaire) {
    Vecteur v(2, -4, 6);

    Vecteur m = v * 0.5;
    EXPECT_NEAR(m.getX(), 1, 1e-10);
    EXPECT_NEAR(m.getY(), -2, 1e-10);
    EXPECT_NEAR(m.getZ(), 3, 1e-10);

    Vecteur d = v / 2.0;
    EXPECT_NEAR(d.getX(), 1, 1e-10);
    EXPECT_NEAR(d.getY(), -2, 1e-10);
    EXPECT_NEAR(d.getZ(), 3, 1e-10);
}
TEST(VecteurTest, OppositeVectors) {
    Vecteur a(3, -4, 1);
    Vecteur b = a * -1;

    Vecteur zero = a + b;

    EXPECT_NEAR(zero.norm(), 0.0, 1e-10);
}
TEST(VecteurTest, DivisionByZero) {
    Vecteur v(1, 2, 3);

    // Expect crash OR handle it
    EXPECT_ANY_THROW({
        Vecteur r = v / 0.0;
    });
}
TEST(VecteurTest, ChainedOperations) {
    Vecteur a(1, 2, 3);
    Vecteur b(1, 1, 1);

    Vecteur c = a + b + b;  // chaining

    EXPECT_NEAR(c.getX(), 3, 1e-10);
    EXPECT_NEAR(c.getY(), 4, 1e-10);
    EXPECT_NEAR(c.getZ(), 5, 1e-10);
}
TEST(VecteurTest, CopyAndAssignment) {
    Vecteur a(1, 2, 3);

    Vecteur b(a);  // copy constructor
    EXPECT_NEAR(b.getX(), 1, 1e-10);
    EXPECT_NEAR(b.getY(), 2, 1e-10);
    EXPECT_NEAR(b.getZ(), 3, 1e-10);

    Vecteur c;
    c = a;  // assignment
    EXPECT_NEAR(c.getX(), 1, 1e-10);
    EXPECT_NEAR(c.getY(), 2, 1e-10);
    EXPECT_NEAR(c.getZ(), 3, 1e-10);
}
TEST(VecteurTest, LargeValues) {
    Vecteur v(1e6, 1e6, 1e6);
    EXPECT_NEAR(v.norm(), std::sqrt(3.0) * 1e6, 1e-3);
}
TEST(VecteurTest, ConsistencyAdd) {
    Vecteur a(1,2,3);
    Vecteur b(4,5,6);

    Vecteur c1 = a + b;

    Vecteur c2 = a;
    c2 += b;

    EXPECT_NEAR(c1.getX(), c2.getX(), 1e-10);
    EXPECT_NEAR(c1.getY(), c2.getY(), 1e-10);
    EXPECT_NEAR(c1.getZ(), c2.getZ(), 1e-10);
}