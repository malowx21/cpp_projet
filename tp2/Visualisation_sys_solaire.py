import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Chargement des données
data = pd.read_csv("build/positions.txt", sep="\t")

fig, ax = plt.subplots(figsize=(8, 8))
ax.set_facecolor("black")
fig.patch.set_facecolor("black")
ax.set_xlim(-15, 15)
ax.set_ylim(-15, 15)
ax.set_aspect("equal")
ax.set_title("Système Solaire", color="white")
ax.tick_params(colors="white")

# Tracés des corps
soleil_pt,  = ax.plot([], [], "o", color="yellow",  markersize=12, label="Soleil")
terre_pt,   = ax.plot([], [], "o", color="blue",    markersize=5,  label="Terre")
jupiter_pt, = ax.plot([], [], "o", color="#C89664",  markersize=8,  label="Jupiter")
halley_pt,  = ax.plot([], [], "o", color="white",   markersize=3,  label="Halley")

# Tracés des trajectoires
QUEUE = 80
soleil_tr,  = ax.plot([], [], "-", color="yellow",  alpha=0.3, linewidth=1)
terre_tr,   = ax.plot([], [], "-", color="blue",    alpha=0.4, linewidth=1)
jupiter_tr, = ax.plot([], [], "-", color="#C89664",  alpha=0.4, linewidth=1)
halley_tr,  = ax.plot([], [], "-", color="white",   alpha=0.4, linewidth=1)

ax.legend(loc="upper right", facecolor="#111111", labelcolor="white")

cols = data.columns.tolist()  # t, Soleil_x, Soleil_y, Terre_x, Terre_y, ...

def update(frame):
    start = max(0, frame - QUEUE)
    tail  = data.iloc[start:frame+1]

    sx = data.iloc[frame, 1]; sy = data.iloc[frame, 2]
    tx = data.iloc[frame, 3]; ty = data.iloc[frame, 4]
    jx = data.iloc[frame, 5]; jy = data.iloc[frame, 6]
    hx = data.iloc[frame, 7]; hy = data.iloc[frame, 8]

    soleil_pt.set_data([sx], [sy])
    terre_pt.set_data([tx], [ty])
    jupiter_pt.set_data([jx], [jy])
    halley_pt.set_data([hx], [hy])

    soleil_tr.set_data(tail.iloc[:, 1], tail.iloc[:, 2])
    terre_tr.set_data(tail.iloc[:, 3], tail.iloc[:, 4])
    jupiter_tr.set_data(tail.iloc[:, 5], tail.iloc[:, 6])
    halley_tr.set_data(tail.iloc[:, 7], tail.iloc[:, 8])

    return soleil_pt, terre_pt, jupiter_pt, halley_pt, \
           soleil_tr, terre_tr, jupiter_tr, halley_tr

ani = animation.FuncAnimation(
    fig, update,
    frames=len(data),
    interval=20,
    blit=True
)

plt.tight_layout()
plt.show()