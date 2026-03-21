#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <list>
#include <chrono>
#include <stdexcept>
#include <SFML/Graphics.hpp>


// Définition de la classe Particule 
class Particule {
    //  Les attributs de la classe 
    double x, y, z, vitessex, vitessey, vitessez, masse, forcex, forcey, forcez;
    int ident;
    std::string cat;

public:
    // Constructeur de la classe Particule 
    Particule(double x, double y, double z,double masse,double vitessex, double vitessey, double vitessez,
              double forcex,   double forcey,   double forcez,int ident, std::string cat){
        this->x = x;   
        this->y= y;
        this->z= z;
        this->vitessex = vitessex; 
        this->vitessey = vitessey;
        this->vitessez = vitessez;
        this->forcex = forcex;   
        this->forcey = forcey;   
        this->forcez = forcez;
        this->masse = masse;
        this->ident = ident;
        this->cat = cat;
    }

    // Définition des geteurs 
    double getX()const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    double getVitesseX() const {return vitessex;}
    double getVitesseY() const {return vitessey;}
    double getVitesseZ() const {return vitessez;}
    double getMasse() const { return masse; }
    double getForceX() const { return forcex;}
    double getForceY() const { return forcey;}
    double getForceZ()  const { return forcez; }
    int    getIdent()  const { return ident; }
    const std::string& getCat() const { return cat; }

    // Affichage de la position de la particule 
    void afficherPosition() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    // Définition des setter 
    void setPosition(double nx, double ny, double nz) {
        x = nx; 
        y = ny; 
        z = nz;
    }
    void setVitesse(double vx, double vy, double vz) {
        vitessex = vx; 
        vitessey = vy;
        vitessez = vz;
    }
    void setForce(double fx, double fy, double fz) {
        forcex = fx; 
        forcey = fy; 
        forcez = fz;
    }
    void setMasse(double m) {
        // Tester si les masses sont négatives
        if (m <= 0)
            throw std::invalid_argument("La masse doit être positive.");
        masse = m;
    }
    void setIdent(int id){ ident = id; }
    void setCat(const std::string& c) { cat = c; }
};


// Comparaison entre list et vector 
std::vector<Particule> collection(int n) {
    std::vector<Particule> particules;
    std::vector<std::string> categories = {"neutron", "proton", "electron"};
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> distPosition(-100.0, 100.0);
    std::uniform_real_distribution<double> distVitesse(-100, 100);
    std::uniform_real_distribution<double> distForce(-100, 100);
    std::uniform_real_distribution<double> distMasse(0.1, 100);
    std::uniform_int_distribution<int> distCat(0, 2);

    for (int i = 0; i < n; i++) {
        particules.emplace_back(distPosition(mt), distPosition(mt), distPosition(mt),distMasse(mt),
            distVitesse(mt),distVitesse(mt),distVitesse(mt),distForce(mt),distForce(mt),distForce(mt),
            i, categories[distCat(mt)] );
    }
    return particules;
}

std::list<Particule> collectionList(int n) {
    std::list<Particule> particules;
    std::vector<std::string> categories = {"neutron", "proton", "electron"};
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> distPosition(-100.0, 100.0);
    std::uniform_real_distribution<double> distVitesse(-100, 100);
    std::uniform_real_distribution<double> distForce(-100, 100);
    std::uniform_real_distribution<double> distMasse(0.1, 100);
    std::uniform_int_distribution<int> distCat(0, 2);

    for (int i = 0; i < n; i++) {
        particules.emplace_back(distPosition(mt), distPosition(mt), distPosition(mt),distMasse(mt),
            distVitesse(mt), distVitesse(mt),distVitesse(mt),distForce(mt),distForce(mt),distForce(mt),
            i, categories[distCat(mt)]);
    }
    return particules;
}

// Création de la structure Vecteur 
struct Vecteur { double fx, fy, fz; };


// Calcul des forces d'interaction graviationnelle entre les particules 
void calculerForces(std::vector<Particule>& particules) {
    for (size_t i = 0; i < particules.size(); ++i) {
        //Initialisation des forces 
        double sumFx = 0;
        double sumFy = 0;
        double sumFz = 0;
        for (size_t j = 0; j < particules.size(); ++j) {
            // calcul de la force gravitionnelle entre les particules i et j 
            if (i == j) continue;
            double dx = particules[j].getX() - particules[i].getX();
            double dy = particules[j].getY() - particules[i].getY();
            double dz = particules[j].getZ() - particules[i].getZ();
            double r = sqrt(dx*dx + dy*dy + dz*dz);
            double r3 = std::pow(r, 3);
            if (r3 > 1e-9) {
                double facteur = (particules[i].getMasse() * particules[j].getMasse()) / r3;
                sumFx += facteur * dx;
                sumFy += facteur * dy;
                sumFz += facteur * dz;
            }
        }
        particules[i].setForce(sumFx, sumFy, sumFz);
    }
}


// Algorithme de Störmer-Verlet avec la sauvegarde des positions dans un fichier de sortie
void algorithmeStormerVerlet(std::vector<Particule>& particules,double dt, double t_end,const std::string& fichierSortie){
    double t = 0;
    calculerForces(particules);
    std::vector<Vecteur> F_old(particules.size());
    // Ouverture du fichier de sortie (Q6)
    std::ofstream ofs;
    bool sauvegarder = !fichierSortie.empty();
    if (sauvegarder) {
        ofs.open(fichierSortie);
        ofs << "t";
        for (const auto& p : particules)
            ofs << "\t" << p.getCat() << "_x\t" << p.getCat() << "_y";
        ofs << "\n";
        
        
    }

    while (t < t_end) {
        t += dt;
        for (size_t i = 0; i < particules.size(); ++i) {
            Particule& p = particules[i];
            double m = p.getMasse();
            p.setPosition(
                p.getX() + dt * (p.getVitesseX() + 0.5 / m * p.getForceX() * dt),
                p.getY() + dt * (p.getVitesseY() + 0.5 / m * p.getForceY() * dt),
                p.getZ() + dt * (p.getVitesseZ() + 0.5 / m * p.getForceZ() * dt)
            );
            F_old[i] = {p.getForceX(), p.getForceY(), p.getForceZ()};
        }
        calculerForces(particules);
        for (size_t i = 0; i < particules.size(); ++i) {
            Particule& p = particules[i];
            double m = p.getMasse();
            p.setVitesse(p.getVitesseX() + dt * 0.5 / m * (p.getForceX() + F_old[i].fx),
                p.getVitesseY() + dt * 0.5 / m * (p.getForceY() + F_old[i].fy),
                p.getVitesseZ() + dt * 0.5 / m * (p.getForceZ() + F_old[i].fz)
            );
        }
        std::cout << "t=" << t;
        for (const auto& p : particules)
            std::cout << "  " << p.getCat() << ":(" << p.getX() << "," << p.getY() << ")";
        std::cout << "\n";
        if (sauvegarder && ofs.is_open()) {
            ofs << t;
            for (const auto& p : particules)
                ofs << "\t" << p.getX() << "\t" << p.getY();
            ofs << "\n";
        }
    }
}

// Définition de la struct état pour la simulation d'après le foramt du fichier de sortie 
struct Etat {double t;double sx, sy;double tx, ty;double jx, jy;double hx, hy;};
// Simulation gravitationnelle du système solaire 
int main() {
    std::vector<Particule> systeme;
    systeme.emplace_back(0,0, 0, 1.0, 0, 0, 0, 0, 0, 0, 0, "Soleil");
    systeme.emplace_back(0, 1,0, 3.0e-6, -1.0,    0, 0, 0, 0, 0, 1, "Terre");
    systeme.emplace_back(0,5.36, 0, 9.55e-4,-0.425,  0, 0, 0, 0, 0, 2, "Jupiter");
    systeme.emplace_back(34.75, 0,0, 1.0e-14, 0, 0.0296, 0, 0, 0, 0, 3, "Haley");

    double dt = 0.015;
    double t_end = 468.5;
    algorithmeStormerVerlet(systeme, dt, t_end, "positions.txt");
    std::cout << "Simulation terminée\n";

    //  Comparaison des performances vector vs list
    std::cout << "\nComparaison de  vector vs list \n";
    int tailles[] = {64, 128, 256, 512, 1024, 2048};

    std::cout << " Pour std::vector \n";
    for (int n : tailles) {
        auto start = std::chrono::steady_clock::now();
        auto particules = collection(n);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "  n=" << n << " : " << elapsed.count() << " s\n";
    }

    std::cout << "Pour std::list \n";
    for (int n : tailles) {
        auto start = std::chrono::steady_clock::now();
        auto particules = collectionList(n);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "  n=" << n << " : " << elapsed.count() << " s\n";
    }
        std::ifstream file("positions.txt");

    std::vector<Etat> data;
    std::string header;
    std::getline(file, header); // ignorer entête
    Etat e;
    while(file >> e.t >> e.sx >> e.sy >> e.tx >> e.ty >> e.jx >> e.jy >> e.hx >> e.hy)
        data.push_back(e);
    sf::RenderWindow window(sf::VideoMode(800,800), "Systeme solaire");
    sf::CircleShape soleil(6);
    soleil.setFillColor(sf::Color::Yellow);
    sf::CircleShape terre(3);
    terre.setFillColor(sf::Color::Blue);
    sf::CircleShape jupiter(5);
    jupiter.setFillColor(sf::Color(200,150,100));

    sf::CircleShape halley(2);
    halley.setFillColor(sf::Color::White);

    float scale = 50;

    int i = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();

        auto &d = data[i];

        soleil.setPosition(400 + d.sx*scale, 400 + d.sy*scale);
        terre.setPosition(400 + d.tx*scale, 400 + d.ty*scale);
        jupiter.setPosition(400 + d.jx*scale, 400 + d.jy*scale);
        halley.setPosition(400 + d.hx*scale, 400 + d.hy*scale);

        window.draw(soleil);
        window.draw(terre);
        window.draw(jupiter);
        window.draw(halley);

        window.display();

        i = (i+1) % data.size();

        sf::sleep(sf::milliseconds(20));
    }

    return 0;
}