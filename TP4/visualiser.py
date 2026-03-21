"""
visualiser.py – Visualisation des résultats du TP4
Lit les fichiers générés par le main C++ :
  - potentiel_lj.csv  → tracé statique (Q1)
  - frames.csv        → animation de la collision (Q6)

Usage :
  python3 visualiser.py             # animation complète
  python3 visualiser.py --potentiel # seulement le tracé LJ
"""

import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
from matplotlib.animation import FuncAnimation


#  Q1 – Tracé du potentiel de Lennard-Jones

def tracer_potentiel(fichier="potentiel_lj.csv"):
    try:
        data = np.loadtxt(fichier, delimiter=",", skiprows=1)
    except FileNotFoundError:
        print(f"Fichier '{fichier}' introuvable. Lance d'abord ./simulation")
        return

    r, U = data[:, 0], data[:, 1]

    fig, ax = plt.subplots(figsize=(8, 5))
    ax.plot(r, U, "b-", linewidth=2)
    ax.axhline(0, color="k", linewidth=0.8, linestyle="--")
    ax.axvline(2**(1/6), color="r", linewidth=1, linestyle=":",
               label=r"$r^* = 2^{1/6}\sigma$ (min)")
    ax.set_ylim(-6, 4)
    ax.set_xlabel(r"$r / \sigma$", fontsize=13)
    ax.set_ylabel(r"$U(r) / \varepsilon$", fontsize=13)
    ax.set_title("Potentiel de Lennard-Jones", fontsize=14)
    ax.legend(fontsize=11)
    ax.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig("potentiel_lj.png", dpi=150)
    print("Sauvegarde -> potentiel_lj.png")
    plt.show()


#  Lecture de frames.csv
#  Retourne une liste de tableaux numpy (x, y, speed)

def lire_frames(fichier="frames.csv"):
    frames = []
    times  = []
    current = []

    try:
        with open(fichier) as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue
                if line.startswith("# t="):
                    if current:
                        frames.append(np.array(current, dtype=float))
                        current = []
                    times.append(float(line[4:]))
                elif line.startswith("#"):
                    continue  # en-tête général
                else:
                    vals = line.split(",")
                    if len(vals) == 3:  # on attend exactement x,y,speed
                        try:
                            current.append([float(v) for v in vals])
                        except ValueError:
                            pass  # ligne corrompue, on ignore
        if current:
            frames.append(np.array(current, dtype=float))
    except FileNotFoundError:
        print(f"Fichier '{fichier}' introuvable. Lance d'abord ./simulation")
        return [], []

    print(f"  {len(frames)} frames chargées, "
          f"{len(frames[0]) if frames else 0} particules")
    return frames, times


#Q6 – Animation de la collision

def animer_collision(fichier="frames.csv", interval_ms=40):
    frames, times = lire_frames(fichier)
    if not frames:
        return

    N        = len(frames[0])
    N_rouge  = 40 * 40   # 1600 particules rouges (créées en premier)
    N_bleu   = 160 * 40  # 6400 particules bleues

    # Bornes spatiales : calculées sur toutes les frames
    all_x = np.concatenate([f[:, 0] for f in frames])
    all_y = np.concatenate([f[:, 1] for f in frames])
    x_min, x_max = all_x.min() - 2, all_x.max() + 2
    y_min, y_max = all_y.min() - 2, all_y.max() + 2

    # Colormap vitesse
    cmap   = plt.cm.jet
    v_max  = max(np.max(f[:, 2]) for f in frames)
    norm   = mcolors.Normalize(vmin=0, vmax=v_max)

    fig, ax = plt.subplots(figsize=(12, 6))
    ax.set_xlim(x_min, x_max)
    ax.set_ylim(y_min, y_max)
    ax.set_aspect("equal")
    ax.set_facecolor("black")
    ax.set_xlabel("x", color="white")
    ax.set_ylabel("y", color="white")
    fig.patch.set_facecolor("#111111")
    ax.tick_params(colors="white")
    for spine in ax.spines.values():
        spine.set_edgecolor("white")

    # Scatter unique, colorié par vitesse
    scat = ax.scatter([], [], s=3, c=[], cmap=cmap, norm=norm, linewidths=0)

    cbar = fig.colorbar(plt.cm.ScalarMappable(norm=norm, cmap=cmap), ax=ax)
    cbar.set_label("Vitesse", color="white")
    cbar.ax.yaxis.set_tick_params(color="white")
    plt.setp(cbar.ax.yaxis.get_ticklabels(), color="white")

    title = ax.set_title("", color="white", fontsize=12)

    def init():
        scat.set_offsets(np.empty((0, 2)))
        return (scat,)

    def update(frame_idx):
        f = frames[frame_idx]
        xy     = f[:, :2]
        speeds = f[:, 2]
        scat.set_offsets(xy)
        scat.set_array(speeds)
        title.set_text(f"Collision LJ  |  t = {times[frame_idx]:.4f}  "
                       f"|  frame {frame_idx+1}/{len(frames)}")
        return (scat,)

    ani = FuncAnimation(fig, update, frames=len(frames),
                        init_func=init, interval=interval_ms, blit=True)

    plt.tight_layout()
    plt.show()
    return ani   # garder la référence pour éviter le garbage collect


#  main

if __name__ == "__main__":
    if "--potentiel" in sys.argv:
        tracer_potentiel()
    else:
        tracer_potentiel()          # Q1 
        animer_collision()          # Q6 
