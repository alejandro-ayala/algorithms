import os

# Ruta del archivo de entrada
input_filename = "logLidarData.log"

# Prefijo para los archivos de salida
output_prefix = "LidarSample_"

# Patrones de inicio y fin
start_marker = "[DEBUG] 3D image captured in"
end_marker = "[INFO] SystemTasks::image3dCapturerTask executed in"

# Variables de control
capturing = False
block_lines = []
block_count = 0

with open(input_filename, "r", encoding="utf-8") as infile:
    for line in infile:
        if start_marker in line:
            capturing = True
            block_lines = []  # No incluir start_marker
        elif end_marker in line and capturing:
            # Guardar el bloque (no incluir end_marker)
            block_count += 1
            output_filename = f"{output_prefix}{block_count}.txt"
            with open(output_filename, "w", encoding="utf-8") as outfile:
                outfile.writelines(block_lines)
            capturing = False  # Reiniciar
        elif capturing:
            # Si la línea empieza con '[INFO]', la transformamos
            if line.startswith("[INFO]"):
                # Quitamos '[INFO] ' y saltos
                data_str = line[len("[INFO] "):].strip()
                # Reemplazamos comas por espacios
                data_str = data_str.replace(",", " ")
                # Convertimos cada valor a float y formateamos a 'x.0'
                valores = data_str.split()
                valores_float_str = [f"{float(v):.1f}" for v in valores]
                # Unimos de nuevo con espacios y agregamos salto de línea
                linea_procesada = " ".join(valores_float_str) + "\n"
                block_lines.append(linea_procesada)
            else:
                block_lines.append(line)


print(f"Se extrajeron {block_count} bloques sin líneas de marca.")
