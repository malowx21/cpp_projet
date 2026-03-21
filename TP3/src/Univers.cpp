#include "Univers.hpp"
#include <vector>
#include "Particule.hpp"
#include <iostream>
#include <cmath>
#include "Vecteur.hpp"
Univers::Univers(int dim) : dimension(dim), t(0.0) {}

void Univers::calcul_forces()
{   
    double eps = 0.05;  //parametre de softening
    double G = 1.0;     
    int N = particules.size();
    double softening = eps * eps;
    //Reinitialiser des forces
    for(auto& p : particules)
        p.force = Vecteur(0,0,0);

    //Calculer les forces gravitationelle
    for(int i = 0; i < N; i++)
    {
        for(int j = i + 1; j < N; j++)
        {
            Vecteur rij = particules[j].position - particules[i].position;
            
            double dist = rij.norm();
            
            
            // F = G * m1 * m2 / r^2  avec softening
            double factor = G * particules[i].m * particules[j].m / 
                           (dist * dist * dist + softening);
            
            // Force sur i par j
            Vecteur F = rij * factor;
            
            particules[i].force += F;
            particules[j].force -= F;  //par la 3 ème loi de Newton

        }
    }
    
    //Debug
    static int force_debug = 0;
    force_debug++;
    if (force_debug % 100 == 0 && N > 0) {
        std::cout << "Force on p0: (" 
                  << particules[0].force.x << ", " 
                  << particules[0].force.y << ", " 
                  << particules[0].force.z << ")" << std::endl;
    }
}

void Univers::avancer(double dt, double t_end)
{
    if (t >= t_end) return; 
    int N = particules.size();
    if (N == 0) return;

    // Calculate forces
    calcul_forces();
    
    //Integration euler
    for (int i = 0; i < N; i++) {
        // mise àn jour de vitesse et position
        Vecteur acc = particules[i].force / particules[i].m;
        
        
        
        particules[i].vitesse += acc * dt;
        
     
        particules[i].position += particules[i].vitesse * dt;
    }
    
    
    this->t += dt;
    
    //Debug
    static int step = 0;
    step++;
    if (step % 10 == 0 && N > 0) {
        std::cout << "Step " << step << ": p0 at (" 
                  << particules[0].position.x << ", " 
                  << particules[0].position.y << ", " 
                  << particules[0].position.z << ")" 
                  << " |v| = " << particules[0].vitesse.norm()
                  << " |a| = " << (particules[0].force/particules[0].m).norm()
                  << std::endl;
    }
}