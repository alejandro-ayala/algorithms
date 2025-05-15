import os

# Verificar si el directorio existe
#input_folder = "D:/Code/algorithms/Resources/imageSamples/out"
#Soutput_folder = "D:/Code/algorithms/Resources/imageSamples/out"
input_folder = "C:/Users/aayal/Documents/Code/algorithms/Resources/imageSamples/out"
output_folder = "C:/Users/aayal/Documents/Code/algorithms/Resources/imageSamples/out"

if not os.path.exists(input_folder):
    print(f"Error: El directorio {input_folder} no existe.")
else:
    print(f"El directorio {input_folder} existe.")

for filename in os.listdir(input_folder):
    if filename.endswith(".raw"):
        raw_path = os.path.join(input_folder, filename)
        array_name = os.path.splitext(filename)[0]

        with open(raw_path, "rb") as f:
            data = f.read()

        header_filename = os.path.join(output_folder, f"{array_name}.h")
        with open(header_filename, "w") as out:
            out.write(f"unsigned char {array_name}[] = {{\n    ")
            for i, byte in enumerate(data):
                out.write(f"0x{byte:02X}, ")
                if (i + 1) % 12 == 0:
                    out.write("\n    ")
            out.write("\n};\n")
            out.write(f"unsigned int {array_name}_size = {len(data)};\n")

        print(f"Generated: {header_filename}")
