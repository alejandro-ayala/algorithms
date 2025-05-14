#include "ImageScaler.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize2.h"

#include <iostream>
#include <fstream>

unsigned char* ImageScaler::loadImage(const std::string& filename, int &width, int &height, int &channels)
{
    // Cargar imagen JPEG en RGB
    unsigned char* input_image = stbi_load(filename.c_str(), &width, &height, &channels, 3);

    if (!input_image) {
        std::cerr << "Error cargando la imagen: " << filename << std::endl;
    }

    std::cout << "Imagen cargada: " << width << "x" << height << " - Canales: " << channels << std::endl;
    return input_image;
}

void ImageScaler::saveImage(const std::string& filename, unsigned char* outputBuffer, int bufferSize)
{
    // Guardar como RAW en gris (1 byte por píxel)
    std::ofstream outFile(filename.c_str(), std::ios::binary);
    outFile.write(reinterpret_cast<char*>(outputBuffer), bufferSize);
    outFile.close();

    std::cout << "Imagen en gris escalada guardada como imagen_escalada_gray.raw" << std::endl;
}

void ImageScaler::rgb2Gray(const unsigned char* rgb, unsigned char* gray, int width, int height) 
{
    for (int i = 0; i < width * height; ++i) {
        int r = rgb[i * 3 + 0];
        int g = rgb[i * 3 + 1];
        int b = rgb[i * 3 + 2];
        // Fórmula estándar de luminosidad perceptual
        gray[i] = static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
    }
}

void ImageScaler::scaleBilinear(uint8_t *input, uint8_t *output, int input_width, int input_height, int output_width, int output_height) 
{
    // Escalar imagen en escala de grises usando la nueva API (STBIR_GREY)
    stbir_resize_uint8_linear(
        input, input_width, input_height, 0,
        output, output_width, output_height, 0,
        STBIR_1CHANNEL
    );
    std::cout << "Imagen convertida y escalada a gris " << output_width << "x" << output_height << std::endl;
}
