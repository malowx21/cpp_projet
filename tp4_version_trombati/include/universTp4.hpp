#pragma once
#include <iostream>
#include <vector>
#include <celluleTp4.hpp>
#include <vecteur.hpp>
#include <particule.hpp>


class UniversTp4 {
    int dim;
    int n_cd;
    double Ld;
    double r_cut ;
    double sigma ;
    double epsilon;
    std::vector<Cellule> cellules ; 
    std::vector<Particule>& collection;
    std::vector<Vecteur> F_old;

    public :
        UniversTp4(int dim , double Ld, double r_cut , double sigma, double epsilon,std::vector<Particule>& p);
        int get_dim() const;
        double get_rcut() const;
        int get_ncd() const;
        double get_Ld() const;
        double get_sigma() const;
        double get_epsilon() const;
        std::vector<Cellule>& getCellules();
        double& operator[](int i); 
        void initialiser_univers_cellules(); //Q3
        void calculer_forces(); //Q4
        void placer_particule(Particule& particule); //Q5
        void maj_cellules(); //Q5
        void avancer(double dt);



};


