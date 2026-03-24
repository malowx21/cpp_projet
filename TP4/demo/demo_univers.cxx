#include "Vecteur.hpp"
#include "Particule.hpp"
#include "Univers.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <vector>
#include <sys/stat.h>

// ============================================================
//  Q1 - Export du potentiel LJ
// ============================================================
void exporter_potentiel_lj(double eps, double sigma, const std::string& fichier) {
    std::ofstream f(fichier);
    f << "r,U\n";
    for (int i = 1; i <= 500; ++i) {
        double r   = 0.85 + i * (4.0 - 0.85) / 500.0;
        double sr6 = std::pow(sigma / r, 6);
        double U   = 4.0 * eps * (sr6 * sr6 - sr6);
        f << r << "," << U << "\n";
    }
    std::cout << "Potentiel LJ exporte -> " << fichier << "\n";
}

// ============================================================
//  Paramètres simulation (Q6)
// ============================================================
const double EPSILON = 5.0;
const double SIGMA   = 1.0;
const double RCUT    = 2.5 * SIGMA;
const double DT      = 0.00005;
const double T_END   = 19.5;
const double EQ_DIST = std::pow(2.0, 1.0 / 6.0) * SIGMA;

// ============================================================
//  Création d'un rectangle de particules
// ============================================================
void creer_rectangle(Univers& U, double x_min, double y_min,
                     int nx, int ny, const Vecteur& v0, int& id_counter) {
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            Particule p;
            p.setPosition(Vecteur(x_min + i * EQ_DIST,
                                  y_min + j * EQ_DIST, 0.0));
            p.setVitesse(v0);
            p.setForce(Vecteur(0, 0, 0));
            p.setMasse(1.0);
            p.setId(id_counter++);
            U.ajouter_particule(p);
        }
    }
}

// ============================================================
//  Écriture d'un fichier VTK PolyData (.vtp) pour une frame
//
//  Format VTK XML PolyData — lisible par ParaView sans lib VTK.
//  Chaque fichier contient :
//    - les positions 3D des particules  (Points)
//    - la norme de la vitesse           (PointData scalar "speed")
//    - le vecteur vitesse               (PointData vector "velocity")
//    - l'identifiant                    (PointData scalar "id")
// ============================================================
void ecrire_vtk(const Univers& univers, int frame_index,
                double t, const std::string& dir) {
    const auto& particules = univers.get_particules();
    int N = static_cast<int>(particules.size());

    // Nom du fichier : vtk_output/particles_000042.vtp
    std::ostringstream oss;
    oss << dir << "/particles_"
        << std::setw(6) << std::setfill('0') << frame_index << ".vtp";
    std::string fichier = oss.str();

    std::ofstream f(fichier);
    if (!f.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir " << fichier << "\n";
        return;
    }

    // --- En-tête XML ---
    f << "<?xml version=\"1.0\"?>\n";
    f << "<VTKFile type=\"PolyData\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    f << "  <!-- timestep=" << t << " -->\n";
    f << "  <PolyData>\n";
    f << "    <Piece NumberOfPoints=\"" << N
      << "\" NumberOfVerts=\"" << N
      << "\" NumberOfLines=\"0\" NumberOfStrips=\"0\" NumberOfPolys=\"0\">\n";

    // --- Positions ---
    f << "      <Points>\n";
    f << "        <DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\">\n";
    for (const auto& p : particules) {
        f << "          "
          << p.getPosition().getX() << " "
          << p.getPosition().getY() << " "
          << p.getPosition().getZ() << "\n";
    }
    f << "        </DataArray>\n";
    f << "      </Points>\n";

    // --- Vertices (obligatoire pour que ParaView affiche les points) ---
    f << "      <Verts>\n";
    f << "        <DataArray type=\"Int64\" Name=\"connectivity\" format=\"ascii\">\n";
    f << "         ";
    for (int i = 0; i < N; ++i) f << " " << i;
    f << "\n        </DataArray>\n";
    f << "        <DataArray type=\"Int64\" Name=\"offsets\" format=\"ascii\">\n";
    f << "         ";
    for (int i = 1; i <= N; ++i) f << " " << i;
    f << "\n        </DataArray>\n";
    f << "      </Verts>\n";

    // --- Données par particule ---
    f << "      <PointData Scalars=\"speed\" Vectors=\"velocity\">\n";

    // Vitesse scalaire (norme) — utilisée pour colorier dans ParaView
    f << "        <DataArray type=\"Float64\" Name=\"speed\" format=\"ascii\">\n";
    for (const auto& p : particules) {
        f << "          " << p.getVitesse().norm() << "\n";
    }
    f << "        </DataArray>\n";

    // Vecteur vitesse (vx, vy, vz)
    f << "        <DataArray type=\"Float64\" Name=\"velocity\""
      << " NumberOfComponents=\"3\" format=\"ascii\">\n";
    for (const auto& p : particules) {
        f << "          "
          << p.getVitesse().getX() << " "
          << p.getVitesse().getY() << " "
          << p.getVitesse().getZ() << "\n";
    }
    f << "        </DataArray>\n";

    // Identifiant de la particule
    f << "        <DataArray type=\"Int32\" Name=\"id\" format=\"ascii\">\n";
    for (const auto& p : particules) {
        f << "          " << p.getId() << "\n";
    }
    f << "        </DataArray>\n";

    f << "      </PointData>\n";
    f << "    </Piece>\n";
    f << "  </PolyData>\n";
    f << "</VTKFile>\n";
}

// ============================================================
//  Écriture du fichier PVD — index temporel pour ParaView
//  Permet de charger toute la simulation d'un coup et d'avoir
//  le vrai axe du temps (File > Open > simulation.pvd).
// ============================================================
void ecrire_pvd(const std::vector<double>& temps_frames,
                const std::string& dir) {
    std::string pvd = dir + "/simulation.pvd";
    std::ofstream f(pvd);
    f << "<?xml version=\"1.0\"?>\n";
    f << "<VTKFile type=\"Collection\" version=\"0.1\">\n";
    f << "  <Collection>\n";
    for (size_t i = 0; i < temps_frames.size(); ++i) {
        f << "    <DataSet timestep=\"" << temps_frames[i]
          << "\" group=\"\" part=\"0\" file=\"particles_"
          << std::setw(6) << std::setfill('0') << i << ".vtp\"/>\n";
    }
    f << "  </Collection>\n";
    f << "</VTKFile>\n";
    std::cout << "Index PVD ecrit -> " << pvd << "\n";
}

// ============================================================
//  main
// ============================================================
int main() {

    // Q1 : potentiel LJ
    exporter_potentiel_lj(EPSILON, SIGMA, "potentiel_lj.csv");

    // Q2/Q3 : univers 2D avec grille de cellules
    Univers U(2, EPSILON, SIGMA, RCUT, {250.0, 200.0, 1.0});

    // Q6 : deux blocs de particules
    int id = 0;

    // Bloc rouge : 40x40 particules, vitesse (0,-10)
    int    nx_r   = 40, ny_r = 40;
    double w_bleu  = (160 - 1) * EQ_DIST;
    double w_rouge = (nx_r  - 1) * EQ_DIST;
    double x_rouge = (w_bleu - w_rouge) / 2.0;
    double y_rouge = (40 + 5) * EQ_DIST;
    creer_rectangle(U, x_rouge, y_rouge, nx_r, ny_r, Vecteur(0, -10, 0), id);

    // Bloc bleu : 160x40 particules, immobile
    creer_rectangle(U, 0.0, 0.0, 160, 40, Vecteur(0, 0, 0), id);

    std::cout << "Particules totales : " << id << "\n";
    U.afficher_stats_grille();

    // Création du répertoire de sortie VTK
    const std::string VTK_DIR = "vtk_output";
#ifdef _WIN32
    _mkdir(VTK_DIR.c_str());
#else
    mkdir(VTK_DIR.c_str(), 0755);
#endif
    std::cout << "Fichiers VTK -> " << VTK_DIR << "/\n";

    // Fréquence de sauvegarde
    const int SAVE_EVERY = 200;

    // Frame initiale (t=0)
    int frame_count = 0;
    std::vector<double> temps_frames;
    ecrire_vtk(U, frame_count, 0.0, VTK_DIR);
    temps_frames.push_back(0.0);
    ++frame_count;

    // Boucle de simulation
    double t    = 0.0;
    int    step = 0;
    auto   t0   = std::chrono::high_resolution_clock::now();

    std::cout << "Simulation en cours (VTK toutes les "
              << SAVE_EVERY << " iterations)...\n";

    while (t < T_END) {
        // Q4 forces LJ + Q5 mise à jour cellules (dans avancer)
        U.avancer(DT, T_END, false);
        t += DT;
        ++step;

        if (step % SAVE_EVERY == 0) {
            ecrire_vtk(U, frame_count, t, VTK_DIR);
            temps_frames.push_back(t);
            ++frame_count;

            double pct     = 100.0 * t / T_END;
            double elapsed = std::chrono::duration<double>(
                std::chrono::high_resolution_clock::now() - t0).count();
            std::cout << "\r  " << std::fixed << std::setprecision(1)
                      << pct << "%  frames=" << frame_count
                      << "  elapsed=" << elapsed << "s    " << std::flush;
        }
    }

    // Fichier PVD (index temporel)
    ecrire_pvd(temps_frames, VTK_DIR);

    double total = std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t0).count();
    std::cout << "\nTermine en " << total << " s  |  "
              << frame_count << " frames\n";
    std::cout << "ParaView : File > Open > " << VTK_DIR << "/simulation.pvd\n";

    return 0;
}