#include <iostream>
#include <universTp4.hpp>
#include <celluleTp4.hpp>
#include <particule.hpp>
#include <vecteur.hpp>
#include <stdexcept>
#include <cmath>

UniversTp4::UniversTp4(int dim1 , double Ld1, double r_cut1 , double sigma1, double epsilon1,std::vector<Particule>& p) :
    dim(dim1) ,  Ld(Ld1),  r_cut(r_cut1) ,  sigma(sigma1),  epsilon(epsilon1),collection(p),F_old(p.size(), Vecteur(0,0,0)) {
        n_cd = floor(Ld/r_cut);
        initialiser_univers_cellules();
    }
    
    
int UniversTp4::get_dim() const{ return dim;}
double UniversTp4::get_rcut() const { return r_cut;}
double UniversTp4::get_sigma() const { return sigma;}
double UniversTp4::get_epsilon() const { return epsilon;}
int UniversTp4::get_ncd() const{return n_cd;} 
double UniversTp4::get_Ld() const { return Ld;}
std::vector<Cellule>& UniversTp4::getCellules() { return cellules; }

double& UniversTp4::operator[](int i){
    if (i < 1 || i > 4) throw std::out_of_range("Please enter index between 1 and 4 included");
    if (i==1) return Ld;
    if (i==2) return r_cut;
    if (i==3) return sigma;
    else return epsilon;
}
    
    

void UniversTp4::initialiser_univers_cellules(){
    int nb_cellules = pow(n_cd,dim);
    cellules.resize(nb_cellules);

    if (dim==1){
        for (int i = 0; i < nb_cellules;i++){
            for (int pas=-1; pas<2;pas++ ){
                int j  = i + pas;
                if (0<=j && j<nb_cellules){
                    cellules[i].addVoisin(&cellules[j]);
                } 
            }
        }
    }

    else  {
        for (int i = 0; i < n_cd;i++){
            for (int j = 0; j < n_cd;j++){
                for (int pas_x = -1; pas_x <= 1; pas_x++) {
                    for (int pas_y = -1; pas_y <= 1; pas_y++) {
                        int k_x = i + pas_x;
                        int k_y = j + pas_y;
                        if (k_x >= 0 && k_x < n_cd && k_y>= 0 && k_y< n_cd){
                            cellules[i+ n_cd*j].addVoisin(&cellules[k_x+n_cd*k_y]);
                        }

                    }
                }
            }
        }
    }
    // il me reste dim3 
}


void UniversTp4::calculer_forces() {
    for (auto& p : collection){
        p[6] = 0;
        p[7] = 0;
        p[8] = 0;
    } 
    for (auto& cellule : cellules) {
        for (Cellule* voisine : cellule.get_cellule_voisines()) {
            if (voisine < &cellule) continue;
            for (Particule& pi : cellule.get_particule()) {
                for (Particule& pj : voisine->get_particule()) {
                    if (pi.getIdent() == pj.getIdent()) continue;
                    double dx = pi[0] - pj[0];
                    double dy = pi[1] - pj[1];
                    double dz = pi[2] - pj[2];
                    double rij = sqrt(dx*dx + dy*dy + dz*dz);
 
                    if (rij == 0 || rij > r_cut) continue;
                    double c  = pow(sigma/rij,6);
                    double f = 24 * epsilon / pow(rij,2) * c * (1 - 2*c);
 
                    pi[6] += f * dx;
                    pi[7] += f * dy;
                    pi[8] += f * dz;

                    pj[6]-= f * dx;
                    pj[7]-= f * dy;
                    pj[8]-= f * dz;

                }
            }
        }
    }
}


void UniversTp4::placer_particule(Particule& particule){

    double taille_cellule = Ld / n_cd;
    int temp = int(particule[0]/taille_cellule);
    int indice =  std::max(0,std::min(temp,n_cd-1));
    if ( dim==1 ){
        cellules[indice].addParticule(particule);
    }else {
        int temp2 = int(particule[1]/taille_cellule);
        int indice2 =  std::max(0,std::min(temp2,n_cd-1));
        cellules[indice+ n_cd*indice2].addParticule(particule);

    }

    // il  me reste dim 3 

}


void UniversTp4::maj_cellules(){
    for (auto& cellule : cellules){
        cellule.clearParticules();
    }
    for (auto& particule : collection){
        placer_particule(particule);
    }
}
 


void UniversTp4::avancer(double dt){
    calculer_forces();
    
    for (int i=0;i<(int)collection.size();i++){
        collection[i][0]= collection[i][0]+ dt*(collection[i][3]+(0.5/collection[i][9])*collection[i][6]*dt);
        collection[i][1]= collection[i][1]+ dt*(collection[i][4]+(0.5/collection[i][9])*collection[i][7]*dt);
        collection[i][2]= collection[i][2]+ dt*(collection[i][5]+(0.5/collection[i][9])*collection[i][8]*dt);
        F_old[i][0]= collection[i][6];
        F_old[i][1]= collection[i][7];
        F_old[i][2]= collection[i][8];
    }
    calculer_forces();
    for (int i=0;i<(int)collection.size();i++){
        collection[i][3] = collection[i][3]+ dt* (0.5/collection[i][9])* (collection[i][6]+F_old[i][0]);
        collection[i][4] = collection[i][4]+ dt* (0.5/collection[i][9])* (collection[i][7]+F_old[i][1]);
        collection[i][5] = collection[i][5]+ dt* (0.5/collection[i][9])* (collection[i][8]+F_old[i][2]);
    }

}
    
    
    