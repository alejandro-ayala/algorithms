import tkinter as tk
from tkinter import filedialog
from PIL import Image, ImageTk, ImageDraw, ImageFont

class ImageMarkerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Selector de píxeles PNG + Distancia")

        # --- UI Layout ---
        control_frame = tk.Frame(root)
        control_frame.pack(side=tk.TOP, fill=tk.X)

        tk.Label(control_frame, text="X:").pack(side=tk.LEFT)
        self.entry_x = tk.Entry(control_frame, width=5)
        self.entry_x.pack(side=tk.LEFT, padx=2)

        tk.Label(control_frame, text="Y:").pack(side=tk.LEFT)
        self.entry_y = tk.Entry(control_frame, width=5)
        self.entry_y.pack(side=tk.LEFT, padx=2)

        tk.Label(control_frame, text="Distancia:").pack(side=tk.LEFT)
        self.entry_distance = tk.Entry(control_frame, width=10)
        self.entry_distance.pack(side=tk.LEFT, padx=2)

        tk.Button(control_frame, text="Marcar punto", command=self.mark_manual_point).pack(side=tk.LEFT, padx=8)
        tk.Button(control_frame, text="Abrir imagen", command=self.open_image).pack(side=tk.LEFT)
        tk.Button(control_frame, text="Guardar imagen", command=self.save_image).pack(side=tk.LEFT, padx=8)

        self.canvas = tk.Canvas(root)
        self.canvas.pack(fill=tk.BOTH, expand=True)

        self.image = None
        self.tk_image = None
        self.draw = None
        self.font = ImageFont.load_default()

        self.canvas.bind("<Button-1>", self.on_click)

    def open_image(self):
        file_path = filedialog.askopenfilename(
            filetypes=[("PNG files", "*.png")]
        )
        if file_path:
            self.image = Image.open(file_path).convert("RGB")
            self.draw = ImageDraw.Draw(self.image)
            self.display_image()

    def display_image(self):
        self.tk_image = ImageTk.PhotoImage(self.image)
        self.canvas.config(width=self.tk_image.width(), height=self.tk_image.height())
        self.canvas.create_image(0, 0, anchor=tk.NW, image=self.tk_image)

    def draw_point_with_text(self, x, y, distance_text):
        if self.draw:
            self.draw.rectangle((x - 1, y - 1, x + 1, y + 1), fill="red")

            label = f"(x={x}, y={y}) distancia={distance_text} cm"

            # Obtener tamaño del texto usando getbbox (Pillow >= 10)
            bbox = self.font.getbbox(label)
            text_width = bbox[2] - bbox[0]
            text_height = bbox[3] - bbox[1]

            img_width, img_height = self.image.size
            text_x = x + 5
            text_y = y - text_height - 5

            if text_x + text_width > img_width:
                text_x = x - text_width - 5
            if text_y < 0:
                text_y = y + 5

            self.draw.text((text_x, text_y), label, fill="yellow", font=self.font)
            self.display_image()



    def on_click(self, event):
        x, y = event.x, event.y
        print(f"Clic en: ({x}, {y})")
        self.entry_x.delete(0, tk.END)
        self.entry_x.insert(0, str(x))
        self.entry_y.delete(0, tk.END)
        self.entry_y.insert(0, str(y))
        distance = self.entry_distance.get()
        self.draw_point_with_text(x, y, distance)

    def mark_manual_point(self):
        try:
            x = int(self.entry_x.get())
            y = int(self.entry_y.get())
            distance = self.entry_distance.get()
            print(f"Marcando punto manual: ({x}, {y}) con distancia = {distance} cm")
            self.draw_point_with_text(x, y, distance)
        except ValueError:
            print("❌ Coordenadas inválidas o distancia no definida correctamente.")

    def save_image(self):
        if self.image:
            file_path = filedialog.asksaveasfilename(
                defaultextension=".png",
                filetypes=[("PNG files", "*.png"), ("All files", "*.*")]
            )
            if file_path:
                self.image.save(file_path)
                print(f"Imagen guardada en: {file_path}")

        else:
            print("❌ No hay imagen cargada para guardar.")

if __name__ == "__main__":
    root = tk.Tk()
    app = ImageMarkerApp(root)
    root.mainloop()
