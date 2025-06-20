import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# ------------------------------
archivo_lidar = "lidarSample.txt"
guardar_ply = False
archivo_salida_ply = "nube.ply"
# ------------------------------

datos_lidar = np.loadtxt(archivo_lidar, usecols=(0, 1, 2))

# Conversión a radianes
angulosH = np.radians(datos_lidar[:, 0])
angulosV = np.radians(datos_lidar[:, 1])
distancias = datos_lidar[:, 2]

# Conversión a coordenadas cartesianas
x = distancias * np.sin(angulosV) * np.cos(angulosH)
y = distancias * np.sin(angulosV) * np.sin(angulosH)
z = distancias * np.cos(angulosV)

# Cálculo de límites ajustados con un margen
def margen(limites, porcentaje=0.1):
    rango = limites[1] - limites[0]
    return (limites[0] - rango * porcentaje, limites[1] + rango * porcentaje)

xlim = margen((np.min(x), np.max(x)))
ylim = margen((np.min(y), np.max(y)))
zlim = margen((np.min(z), np.max(z)))

# Visualización 3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
p = ax.scatter(x, y, z, c=z, cmap='viridis', s=3)
fig.colorbar(p, label='Altura (Z)')

ax.set_title("Nube de puntos LIDAR")
ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")

# Ajuste de los ejes
ax.set_xlim(xlim)
ax.set_ylim(ylim)
ax.set_zlim(zlim)

plt.tight_layout()
plt.show()

# Exportar a .ply (opcional)
if guardar_ply:
    with open(archivo_salida_ply, "w") as f:
        f.write("ply\nformat ascii 1.0\n")
        f.write(f"element vertex {len(x)}\n")
        f.write("property float x\nproperty float y\nproperty float z\nend_header\n")
        for xi, yi, zi in zip(x, y, z):
            f.write(f"{xi} {yi} {zi}\n")
    print(f"\n✅ Nube guardada en: {archivo_salida_ply}")
