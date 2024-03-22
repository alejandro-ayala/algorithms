import sys
from PIL import Image
import binascii

if len(sys.argv) != 2:
    print("Uso: python printJpeg.py <nombre_archivo_hexadecimal>")
    sys.exit(1)
    
archivo_hex = sys.argv[1]

# Leer el archivo hexadecimal
with open(archivo_hex, 'r') as f:
    hex_data = f.read().strip().replace(' ', '').replace('\n', '')
    
if len(hex_data) % 2 != 0:
    hex_data += '0'  # Agregar un cero al final de la cadena si es necesario
    
# Convertir el archivo hexadecimal en bytes
byte_data = binascii.unhexlify(hex_data)

# Guardar los bytes en un archivo temporal
with open("imagen_temp.jpg", "wb") as f:
    f.write(byte_data)

# Abrir la imagen utilizando PIL
imagen = Image.open("imagen_temp.jpg")

# Mostrar la imagen
imagen.show()

