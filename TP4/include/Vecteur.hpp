#ifndef VECTEUR_HPP
#define VECTEUR_HPP

class Vecteur
{
private:
    // Attributs
    double x, y, z;

public:
    // Constructeurs
    Vecteur();
    Vecteur(double x_, double y_, double z_);

    // Getters
    double getX() const { 
        return x;
    }
    double getY() const { 
        return y;
    }
    double getZ() const { 
        return z;
    }

    // Setters
    void setX(double x_) { 
        x = x_;
    }
    void setY(double y_) { 
        y = y_;
    }
    void setZ(double z_) { 
        z = z_;
    }

    // Norme
    double norm() const;
    // Operations arthmétiques 
    Vecteur operator+(const Vecteur& v) const;
    Vecteur operator-(const Vecteur& v) const;
    Vecteur operator*(double a) const;
    Vecteur operator/(double a) const;
    // Operateurs += et -=
    Vecteur& operator+=(const Vecteur& v);
    Vecteur& operator-=(const Vecteur& v);
};

#endif
