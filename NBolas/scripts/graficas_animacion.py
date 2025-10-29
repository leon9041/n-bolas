import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os

from matplotlib.offsetbox import OffsetImage, AnnotationBbox
import matplotlib.image as mpimg


# Ruta del archivo .dat
datafile = "results/salida.dat"

# Leer datos
data = np.loadtxt(datafile)
t = data[:, 0]
rest = data[:, 1:]
N = rest.shape[1] // 4
print(f"Cargando datos: {N} partículas, {len(t)} pasos.")

# Crear carpeta results si no existe
os.makedirs("results", exist_ok=True)

# ----------------------------------------------------------
# 1️⃣ Graficar trayectorias
# ----------------------------------------------------------
plt.figure(figsize=(6,6))
for i in range(min(10, N)):
    x = rest[:, 4*i]
    y = rest[:, 4*i + 1]
    plt.plot(x, y, linewidth=0.8, label=f'Bola {i}')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Trayectorias de partículas')
plt.axis('equal')
plt.legend(fontsize='x-small', ncol=2)
plt.grid(True)
plt.tight_layout()
plt.savefig("results/trayectorias.png", dpi=150)
plt.close()

# ----------------------------------------------------------
# ----------------------------------------------------------
# 2️⃣ Histograma de velocidades finales (con Maxwell–Boltzmann)
# ----------------------------------------------------------
vels = np.zeros((len(t), N))
for i in range(N):
    vx = rest[:, 4*i + 2]
    vy = rest[:, 4*i + 3]
    vels[:, i] = np.sqrt(vx**2 + vy**2)

v_final = vels[-1, :]  # velocidades del último paso

# --- Histograma normalizado ---
plt.figure(figsize=(7,5))
counts, bins, _ = plt.hist(v_final, bins=20, density=True,
                           color='lightblue', edgecolor='black', alpha=0.6, label='Simulación')

# --- Calcular curva de Maxwell–Boltzmann (ajustada al promedio) ---
v = np.linspace(0, np.max(v_final)*1.2, 200)
v_mean = np.mean(v_final)
a = v_mean / np.sqrt(np.pi / 2)   # relación empírica entre <v> y parámetro
f_MB = (v / a**2) * np.exp(-v**2 / (2*a**2))   # forma en 2D (normalizada)

plt.plot(v, f_MB, 'r-', lw=2, label='Distribución Maxwell–Boltzmann (ajustada)')

plt.xlabel('|v|')
plt.ylabel('Probabilidad')
plt.title('Distribución de velocidades y comparación con Maxwell–Boltzmann')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)
plt.tight_layout()
plt.savefig("results/histograma_velocidades.png", dpi=150)
plt.close()


# 3️⃣ Gráfica de posición X vs Tiempo
# ----------------------------------------------------------
plt.figure(figsize=(7, 5))
for i in range(min(5, N)):  # mostramos las primeras 5 pokébolas
    x = rest[:, 4*i]
    plt.plot(t, x, label=f'Pokébola {i+1}')
plt.xlabel('Tiempo (s)')
plt.ylabel('Posición en x')
plt.title('Posición en x vs Tiempo')
plt.legend(fontsize='small', ncol=2)
plt.grid(True, linestyle='--', alpha=0.6)
plt.tight_layout()
plt.savefig("results/x_vs_t.png", dpi=150)
plt.close()

# ----------------------------------------------------------
# ----------------------------------------------------------
# # 5️⃣ Presión teórica vs experimental
data_pres = np.loadtxt("results/presion.dat")
t_pres = data_pres[:,0]
P_exp = data_pres[:,1]
P_teo = data_pres[:,2]

plt.figure(figsize=(7,5))
plt.plot(t_pres, P_exp, 'b-', label='Presión experimental (choques)')
plt.plot(t_pres, P_teo, 'r--', label='Presión teórica (∝ nm⟨v²⟩/L²)')
plt.xlabel('Tiempo')
plt.ylabel('Presión (unidades arbitrarias)')
plt.title('Comparación de presión experimental y teórica')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("results/presion_vs_t.png", dpi=150)
plt.close()

# ----------------------------------------------------------
# 3️⃣ Animación (matplotlib.animation)
# ----------------------------------------------------------
fig, ax = plt.subplots(figsize=(6,6))
ax.set_xlim(0, 1)
ax.set_ylim(0, 1)
ax.set_title("Animación del sistema de partículas")
ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_facecolor("black")  # fondo oscuro opcional 💫

# 🔹 Cargar imagen de Pokébola (asegúrate de tener scripts/pokeball.png)
sprite = mpimg.imread("scripts/pokemini.png")

def init():
    return []

def update(frame):
    ax.clear()
    ax.set_xlim(0, 1)
    ax.set_ylim(0, 1)
    ax.set_title("Simulación de Pokébolas rebotando")
    ax.set_facecolor("black")

    # Coordenadas de cada partícula
    x = rest[frame, 0::4]
    y = rest[frame, 1::4]

    # Dibujar cada partícula como Pokébola
    for i in range(N):
        imagebox = OffsetImage(sprite, zoom=0.5)
        ab = AnnotationBbox(imagebox, (x[i], y[i]), frameon=False)
        ax.add_artist(ab)

    # 🔹 Mostrar el tiempo actual (en segundos)
    tiempo_actual = t[frame]
    ax.text(
        0.05, 0.95,
        f"t = {tiempo_actual:.2f} s",
        transform=ax.transAxes,
        fontsize=10,
        color='white',
        verticalalignment='top',
        bbox=dict(facecolor='black', alpha=0.5, boxstyle='round')
    )

    return []

# 🔹 Submuestreo para que vaya más rápido
skip = 30   # toma 1 de cada 10 cuadros
frames = range(0, len(t), skip)

ani = animation.FuncAnimation(
    fig, update, frames=frames,
    init_func=init, blit=False, interval=30, repeat=False
)

# Guardar animación como GIF
output_anim = "results/animacion.gif"
print(f"Guardando animación en {output_anim} (puede tardar unos segundos)...")
ani.save(output_anim, writer="pillow", fps=30)

print("\n Gráficas y animación guardadas en carpeta results/")





















