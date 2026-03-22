"""
visualiser_tombati.py  –  sans pandas, uniquement numpy + matplotlib
Lit simulation_data.csv produit par simulation.cxx
Format : t,x,y,couleur

Usage : python3 visualiser_tombati.py
"""

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation


#  Lecture manuelle du CSV (sans pandas)

print("Chargement de simulation_data.csv ...")

try:
    with open("simulation_data.csv") as f:
        lines = f.readlines()
except FileNotFoundError:
    print("Fichier 'simulation_data.csv' introuvable.")
    print("Lance d'abord : ./simulation  (dans le dossier build/)")
    exit(1)

t_vals, x_vals, y_vals, cat_vals = [], [], [], []

for line in lines[1:]:   # ignorer l'en-tête
    line = line.strip()
    if not line:
        continue
    parts = line.split(",")
    if len(parts) != 4:
        continue
    try:
        t_vals.append(float(parts[0]))
        x_vals.append(float(parts[1]))
        y_vals.append(float(parts[2]))
        cat_vals.append(parts[3].strip())
    except ValueError:
        continue

t_arr   = np.array(t_vals)
x_arr   = np.array(x_vals)
y_arr   = np.array(y_vals)
cat_arr = np.array(cat_vals)

print(f"  {len(t_arr)} lignes chargées")

temps    = np.unique(t_arr)
N_frames = len(temps)
cats     = np.unique(cat_arr)
print(f"  {N_frames} frames  |  catégories : {cats}")

x_min, x_max = x_arr.min() - 2, x_arr.max() + 2
y_min, y_max = y_arr.min() - 2, y_arr.max() + 2


#  Pré-calcul des frames en mémoire

palette = {"rouge": "#e74c3c", "bleu": "#2e86de"}

frames_data = {}
for t_val in temps:
    mask = t_arr == t_val
    frames_data[t_val] = {}
    for cat in cats:
        m = mask & (cat_arr == cat)
        frames_data[t_val][cat] = (
            np.column_stack((x_arr[m], y_arr[m])) if np.any(m) else np.empty((0, 2))
        )


#  Animation

fig, ax = plt.subplots(figsize=(13, 6))
ax.set_facecolor("black")
fig.patch.set_facecolor("#111111")
ax.set_xlim(x_min, x_max)
ax.set_ylim(y_min, y_max)
ax.set_aspect("equal")
ax.tick_params(colors="white")
ax.set_xlabel("x", color="white")
ax.set_ylabel("y", color="white")
for sp in ax.spines.values():
    sp.set_edgecolor("white")

scatters = {}
for cat in cats:
    color = palette.get(cat, "#ffffff")
    d     = frames_data[temps[0]][cat]
    sc    = ax.scatter(d[:, 0] if len(d) else [],
                       d[:, 1] if len(d) else [],
                       s=2, c=color, label=cat, linewidths=0)
    scatters[cat] = sc

ax.legend(loc="upper right", fontsize=9,
          facecolor="#222222", labelcolor="white", markerscale=4)
title = ax.set_title("", color="white", fontsize=11)

def update(frame_idx):
    t_val = temps[frame_idx]
    for cat, sc in scatters.items():
        d = frames_data[t_val][cat]
        sc.set_offsets(d if len(d) else np.empty((0, 2)))
    title.set_text(f"Collision LJ  |  t = {t_val:.5f}  "
                   f"|  frame {frame_idx+1}/{N_frames}")
    return list(scatters.values())

ani = animation.FuncAnimation(
    fig, update,
    frames=N_frames,
    interval=40,
    blit=False
)

plt.tight_layout()
plt.show()
