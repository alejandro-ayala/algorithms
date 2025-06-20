import os
from PIL import Image

def crear_imagen_compuesta_y_collage(carpeta_entrada, carpeta_salida, tamaño_max=(500, 500)):
    """
    Lee imágenes en carpeta_entrada que tengan formato 'nombre.png' y 'nombre_proj.bmp',
    crea una imagen combinada para cada par con la normal a la izquierda y la _proj a la derecha,
    guarda las imágenes compuestas en carpeta_salida,
    y luego genera una imagen final apilando verticalmente todas las combinadas.
    """

    if not os.path.exists(carpeta_salida):
        os.makedirs(carpeta_salida)

    archivos = os.listdir(carpeta_entrada)
    originales = [f for f in archivos if f.endswith('.png')]

    imagenes_combinadas = []

    for orig in originales:
        nombre_base = orig[:-4]  # quitamos .png
        proj = f"{nombre_base}_proj.bmp"
        path_orig = os.path.join(carpeta_entrada, orig)
        path_proj = os.path.join(carpeta_entrada, proj)

        if not os.path.exists(path_proj):
            print(f"No se encontró la imagen proyectada para {orig}, saltando.")
            continue

        img_orig = Image.open(path_orig)
        img_proj = Image.open(path_proj)

        img_orig.thumbnail(tamaño_max)
        img_proj.thumbnail(tamaño_max)

        ancho_total = img_orig.width + img_proj.width
        alto_max = max(img_orig.height, img_proj.height)

        imagen_combinada = Image.new('RGB', (ancho_total, alto_max), (255, 255, 255))
        imagen_combinada.paste(img_orig, (0, 0))
        imagen_combinada.paste(img_proj, (img_orig.width, 0))

        salida_path = os.path.join(carpeta_salida, f"{nombre_base}_combinada.png")
        imagen_combinada.save(salida_path)
        print(f"Guardada imagen combinada: {salida_path}")

        imagenes_combinadas.append(imagen_combinada)

    # Ahora creamos la imagen collage apilando verticalmente
    if imagenes_combinadas:
        ancho_collage = max(im.width for im in imagenes_combinadas)
        alto_collage = sum(im.height for im in imagenes_combinadas)

        collage = Image.new('RGB', (ancho_collage, alto_collage), (255, 255, 255))

        y_offset = 0
        for im in imagenes_combinadas:
            collage.paste(im, (0, y_offset))
            y_offset += im.height

        ruta_collage = os.path.join(carpeta_salida, "collage_combinado.png")
        collage.save(ruta_collage)
        print(f"Guardada imagen collage final: {ruta_collage}")
    else:
        print("No se generaron imágenes combinadas para crear el collage.")

if __name__ == "__main__":
    carpeta_entrada = "./imagenes"   # Cambia a la ruta de tus imágenes
    carpeta_salida = "./combinadas"  # Carpeta donde se guardan las imágenes
    crear_imagen_compuesta_y_collage(carpeta_entrada, carpeta_salida)
               