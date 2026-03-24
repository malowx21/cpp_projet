#ifndef EXPORT_VTK_HPP
#define EXPORT_VTK_HPP

#include "Univers.hpp"
#include <fstream>
#include <string>
#include <iomanip>

// Q3 - sauvegarde des particules au format VTK UnstructuredGrid
// le fichier genere peut etre ouvert dans Paraview
void exporter_vtk(const Univers& U, const std::string& fichier) {
    std::ofstream f(fichier);

    const auto& particules = U.getParticules();
    int N = particules.size();

    f << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    f << "  <UnstructuredGrid>\n";
    f << "    <Piece NumberOfPoints=\"" << N << "\" NumberOfCells=\"0\">\n";

    // positions
    f << "      <Points>\n";
    f << "        <DataArray name=\"Position\" type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n";
    f << "          ";
    for (const auto& p : particules)
        f << p.getPosition().getX() << " "
          << p.getPosition().getY() << " "
          << p.getPosition().getZ() << "  ";
    f << "\n        </DataArray>\n";
    f << "      </Points>\n";

    // vitesses et masses
    f << "      <PointData Vectors=\"vector\">\n";

    f << "        <DataArray type=\"Float32\" Name=\"Velocity\" NumberOfComponents=\"3\" format=\"ascii\">\n";
    f << "          ";
    for (const auto& p : particules)
        f << p.getVitesse().getX() << " "
          << p.getVitesse().getY() << " "
          << p.getVitesse().getZ() << "  ";
    f << "\n        </DataArray>\n";

    f << "        <DataArray type=\"Float32\" Name=\"Masse\" format=\"ascii\">\n";
    f << "          ";
    for (const auto& p : particules)
        f << p.getMasse() << " ";
    f << "\n        </DataArray>\n";

    f << "      </PointData>\n";

    // cellules vides (obligatoires dans le format VTK)
    f << "      <Cells>\n";
    f << "        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\"></DataArray>\n";
    f << "        <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\"></DataArray>\n";
    f << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\"></DataArray>\n";
    f << "      </Cells>\n";

    f << "    </Piece>\n";
    f << "  </UnstructuredGrid>\n";
    f << "</VTKFile>\n";
}

// exporte une serie de fichiers vtk_0000.vtu, vtk_0001.vtu ...
// a appeler a chaque iteration qu'on veut sauvegarder
void exporter_vtk_serie(const Univers& U, int step) {
    std::ostringstream nom;
    nom << "vtk_" << std::setw(4) << std::setfill('0') << step << ".vtu";
    exporter_vtk(U, nom.str());
}

#endif
