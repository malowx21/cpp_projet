#include<set>
#include<list>
#include<deque>
#include<vector>
#include<random>
#include<iostream>
#include <chrono>


#include <vector>
#include <cmath>

// void stormer_verlet(std::vector<Particule>& particles,
//     double dt,
//     double t_end)
// {
//     double t = 0.0;
//     int N = particles.size();

//     // Initial force computation
//     compute_forces(particles);

//     while (t < t_end)
//     {
//     t += dt;

//     // Store old forces
//     std::vector<double> fx_old(N), fy_old(N), fz_old(N);

//     for (int i = 0; i < N; i++)
//     {
//         fx_old[i] = particles[i].fx;
//         fy_old[i] = particles[i].fy;
//         fz_old[i] = particles[i].fz;
//     }

//     // 1 Update positions
//     for (int i = 0; i < N; i++)
//     {
//         particles[i].x += dt * (particles[i].vx +
//                             0.5 * fx_old[i] / particles[i].m * dt);

//         particles[i].y += dt * (particles[i].vy +
//                             0.5 * fy_old[i] / particles[i].m * dt);

//         particles[i].z += dt * (particles[i].vz +
//                             0.5 * fz_old[i] / particles[i].m * dt);
//     }

//     // 2️⃣ Recompute forces at new positions
//     compute_forces(particles);

//     // 3️⃣ Update velocities
//     for (int i = 0; i < N; i++)
//     {
//         particles[i].vx += dt * 0.5 / particles[i].m *
//                     (particles[i].fx + fx_old[i]);

//         particles[i].vy += dt * 0.5 / particles[i].m *
//                     (particles[i].fy + fy_old[i]);

//         particles[i].vz += dt * 0.5 / particles[i].m *
//                     (particles[i].fz + fz_old[i]);
//     }

//     // Optional: display positions
//     for (const auto& p : particles)
//     p.AffichePosition();

//     std::cout << "------ t = " << t << " ------\n";
//     }
// }



// void compute_forces(std::vector<Particule>& particles)
// {
//     const double G = 1.0;        // Gravitational constant (scaled)
//     const double epsilon = 1e-9; // Avoid division by zero

//     int N = particles.size();

//     // 1 Reset all forces to zero
//     for (int i = 0; i < N; i++)
//     {
//         particles[i].fx = 0.0;
//         particles[i].fy = 0.0;
//         particles[i].fz = 0.0;
//     }

//     // 2 Compute pairwise gravitational forces
//     for (int i = 0; i < N; i++)
//     {
//         for (int j = i + 1; j < N; j++)
//         {
//             // r_ij = position_j - position_i
//             double dx = particles[j].x - particles[i].x;
//             double dy = particles[j].y - particles[i].y;
//             double dz = particles[j].z - particles[i].z;

//             double dist_sq = dx*dx + dy*dy + dz*dz + epsilon;
//             double dist = std::sqrt(dist_sq);

//             double factor = G * particles[i].m * particles[j].m
//                             / (dist_sq * dist);  // = 1 / r^3

//             double fx = factor * dx;
//             double fy = factor * dy;
//             double fz = factor * dz;

//             // Apply Newton's third law
//             particles[i].fx += fx;
//             particles[i].fy += fy;
//             particles[i].fz += fz;

//             particles[j].fx -= fx;
//             particles[j].fy -= fy;
//             particles[j].fz -= fz;
//         }
//     }
// }

// Particule generer_particule(int id, std::uniform_real_distribution<double> pos_dist,std::mt19937 mt){
   
//     Particule p;
//     p.id = id;

//     p.x = pos_dist(mt);
//     p.y = pos_dist(mt);
//     p.z = pos_dist(mt);
//     return p;





// }



//simple et rapide


    // std::random_device rd; //Génère une source d’entropie (seed “aléatoire”)
    // std::mt19937 mt(rd());//Crée un générateur pseudo-aléatoire basé sur l’algorithme Mersenne 
    // std::uniform_real_distribution<double> pos_dist(-10.0, 10.0);// Définit une loi uniforme continue sur l’intervalle :
    // //vector implementation
    // int N= 512;
    // auto start = std::chrono::high_resolution_clock::now();
    // std::vector<Particule> systeme_solaire_vec;

    // for(int i=0; i<N;i++){
    //     systeme_solaire_vec.push_back(generer_particule(i, pos_dist,mt));
    // }
    
   
    // auto end = std::chrono::high_resolution_clock::now();
    // std::cout << "Vector implementation time  :" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
    //           << " ms\n";
    // //List implementation
    // start = std::chrono::high_resolution_clock::now();
    // std::list<Particule> systeme_solaire_lis;
    // for(int i=0; i<N;i++){
    //     systeme_solaire_lis.push_back(generer_particule(i, pos_dist,mt));
    // }
    
    
    // end = std::chrono::high_resolution_clock::now();
    // std::cout << "List implementation time  :" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
    //           << " ms\n";
    // //Queue implementation
    // start = std::chrono::high_resolution_clock::now();
    // std::deque<Particule> systeme_solaire_que;
    // for(int i=0; i<N;i++){
    //     systeme_solaire_que.push_back(generer_particule(i, pos_dist,mt));
    // }
    
  

    // end = std::chrono::high_resolution_clock::now();
    // std::cout << "Que implementation time  :" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
    //           << " ms\n";
    // // for (const auto& p : systeme_solaire) {
    // //     p.AffichePosition();
    // // }
}