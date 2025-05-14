import cv2
import numpy as np
import os

# Definir las rutas de los archivos
cfg_file = 'yolov3-tiny.cfg'
weights_file = 'yolov3-tiny.weights'
class_file = 'coco.names'

# Cargar las clases
with open(class_file, 'r') as f:
    classes = [line.strip() for line in f.readlines()]

# Cargar la red YOLO
print("[INFO] Cargando la red YOLO...")
net = cv2.dnn.readNet(weights_file, cfg_file)

# Directorio de las imágenes
image_dir = 'resources/'

# Obtener todos los archivos jpeg en el directorio
image_files = [f for f in os.listdir(image_dir) if f.endswith('.jpeg')]

# Verificar si hay imágenes en el directorio
if not image_files:
    print("[ERROR] No se encontraron imágenes en la carpeta 'resources'.")
    exit(1)

# Procesar todas las imágenes
for image_file in image_files:
    # Ruta completa de la imagen
    image_path = os.path.join(image_dir, image_file)
    
    # Cargar la imagen
    image = cv2.imread(image_path)  # Reemplaza con la ruta de la imagen
    if image is None:
        print(f"[ERROR] No se pudo cargar la imagen {image_file}.")
        continue

    height, width = image.shape[:2]

    # Realizar una copia de la imagen para mostrarla más tarde
    blob = cv2.dnn.blobFromImage(image, 0.00392, (416, 416), (0, 0, 0), True, crop=False)

    # Establecer la entrada de la red
    net.setInput(blob)

    # Obtener las capas de salida
    layer_names = net.getLayerNames()
    output_layers = [layer_names[i - 1] for i in net.getUnconnectedOutLayers()]

    # Realizar la detección
    print(f"[INFO] Detectando objetos en {image_file}...")
    outs = net.forward(output_layers)

    # Procesar las detecciones
    class_ids = []
    confidences = []
    bboxes = []
    for out in outs:
        for detection in out:
            scores = detection[5:]
            class_id = np.argmax(scores)
            confidence = scores[class_id]

            if confidence > 0.5:
                # Coordenadas del cuadro delimitador
                center_x = int(detection[0] * width)
                center_y = int(detection[1] * height)
                w = int(detection[2] * width)
                h = int(detection[3] * height)

                # Rectángulo del cuadro delimitador
                x = int(center_x - w / 2)
                y = int(center_y - h / 2)

                bboxes.append([x, y, w, h])
                confidences.append(float(confidence))
                class_ids.append(class_id)

    # Realizar NMS (Non-Maximum Suppression)
    print(f"[INFO] Aplicando NMS a {image_file}...")
    indices = cv2.dnn.NMSBoxes(bboxes, confidences, 0.5, 0.4)

    # Verifica si se han detectado objetos
    if len(indices) > 0:
        for i in indices.flatten():
            # Obtener las coordenadas del cuadro delimitador
            x, y, w, h = bboxes[i]

            # Dibuja el cuadro delimitador y el texto
            label = str(classes[class_ids[i]])
            confidence = confidences[i]
            color = (0, 255, 0)  # Verde
            cv2.rectangle(image, (x, y), (x + w, y + h), color, 2)
            cv2.putText(image, f"{label} {confidence:.2f}", (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

            # Mostrar detalles de depuración
            print(f"[DEBUG] Detección: {label} - Confianza: {confidence:.2f} - Caja: ({x}, {y}, {w}, {h})")

    else:
        print(f"[INFO] No se detectaron objetos en {image_file}.")

    # Guardar la imagen con las detecciones
    output_image_path = os.path.join(image_dir, f"detected_{image_file}")
    cv2.imwrite(output_image_path, image)
    print(f"[INFO] Imagen guardada: {output_image_path}")

    # Mostrar la imagen con las detecciones (opcional)
    # cv2.imshow(f"Detección de objetos - {image_file}", image)
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()

print("[INFO] Detección completada para todas las imágenes.")
