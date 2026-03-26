/**
 * @file Vecteur.hpp
 * @brief Définition de la classe Vecteur 
 */

#ifndef VECTEUR_HPP
#define VECTEUR_HPP

/**
 * @class Vecteur
 * @brief Représente un vecteur à trois composantes (x, y, z).
 *
 * Cette classe fournit les opérations arithmétiques de base ainsi que
 * le calcul de la norme, utilisés notamment pour représenter positions,
 * vitesses et forces des particules.
 */
class Vecteur
{
private:
    double x;
    double y;
    double z;

public:
    /**
     * @brief Constructeur par défaut.
     *
     * Initialise toutes les composantes à 0.
     */
    Vecteur();

    /**
     * @brief Constructeur avec initialisation des composantes.
     * @param x_ Composante x.
     * @param y_ Composante y.
     * @param z_ Composante z.
     */
    Vecteur(double x_, double y_, double z_);

    /**
     * @brief Retourne la composante x.
     * @return Valeur de x.
     */
    double getX() const { 
        return x; 
    }

    /**
     * @brief Retourne la composante y.
     * @return Valeur de y.
     */
    double getY() const { 
        return y;
    }

    /**
     * @brief Retourne la composante z.
     * @return Valeur de z.
     */
    double getZ() const { 
        return z;
    }

    /**
     * @brief Modifie la composante x.
     * @param x_ Nouvelle valeur de x.
     */
    void setX(double x_) { 
        x = x_;
    }

    /**
     * @brief Modifie la composante Y.
     * @param y_ Nouvelle valeur de y.
     */
    void setY(double y_) { 
        y = y_;
    }

    /**
     * @brief Modifie la composante Z.
     * @param z_ Nouvelle valeur de z.
     */
    void setZ(double z_) { 
        z = z_;
    }

    /**
     * @brief Calcule la norme euclidienne du vecteur.
     * @return La norme sqrt(x² + y² + z²).
     */
    double norm() const;

    /**
     * @brief Addition de deux vecteurs.
     * @param v Le vecteur à additionner.
     * @return Un nouveau vecteur résultant de l'addition.
     */
    Vecteur operator+(const Vecteur& v) const;

    /**
     * @brief Soustraction de deux vecteurs.
     * @param v Le vecteur à soustraire.
     * @return Un nouveau vecteur résultant de la soustraction.
     */
    Vecteur operator-(const Vecteur& v) const;

    /**
     * @brief Multiplication par un scalaire.
     * @param a Le scalaire multiplicateur.
     * @return Un nouveau vecteur mis à l'échelle.
     */
    Vecteur operator*(double a) const;

    /**
     * @brief Division par un scalaire.
     * @param a Le scalaire diviseur (ne doit pas être nul).
     * @return Un nouveau vecteur divisé par a.
     */
    Vecteur operator/(double a) const;

    /**
     * @brief Addition en place.
     * @param v Le vecteur à additionner.
     * @return Référence vers ce vecteur modifié.
     */
    Vecteur& operator+=(const Vecteur& v);

    /**
     * @brief Soustraction en place.
     * @param v Le vecteur à soustraire.
     * @return Référence vers ce vecteur modifié.
     */
    Vecteur& operator-=(const Vecteur& v);
};

#endif