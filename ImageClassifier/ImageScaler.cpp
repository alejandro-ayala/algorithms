#include "ImageScaler.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../stb/stb_image_resize2.h"

#include <iostream>
#include <fstream>

ImageScaler::ImageScaler(const std::vector<uint8_t>& input, uint16_t width, uint16_t height, uint8_t channels)
: m_inputImage(input), m_width(width), m_height(height), m_channels(channels) 
{}

bool ImageScaler::loadImage(const std::string& filename)
{
    // Cargar imagen JPEG en RGB
    bool result = true;
    unsigned char* input_image = stbi_load(filename.c_str(), &m_width, &m_height, &m_channels, 3);

    if (!input_image) {
        std::cerr << "Error cargando la imagen: " << filename << std::endl;
        result = false;
    }

    size_t imageSize = static_cast<size_t>(m_width) * m_height * m_channels;
    m_inputImage.assign(input_image, input_image + imageSize);
    stbi_image_free(input_image);

    std::cout << "Imagen cargada: " << m_width << "x" << m_height << " - Canales: " << m_channels << std::endl;
    return result;
}

void ImageScaler::saveImage(const std::string& filename)
{
    // Guardar como RAW en gris (1 byte por píxel)
    std::ofstream outFile(filename.c_str(), std::ios::binary);
    outFile.write(reinterpret_cast<char*>(m_outputImage.data()), m_outputImage.size());
    outFile.close();

    std::cout << "Imagen en gris escalada guardada como imagen_escalada_gray.raw" << std::endl;
}

void ImageScaler::rgb2Gray() 
{
    for (int i = 0; i < m_width * m_height; ++i) {
        int r = m_inputImage[i * 3 + 0];
        int g = m_inputImage[i * 3 + 1];
        int b = m_inputImage[i * 3 + 2];
        // Fórmula estándar de luminosidad perceptual
        m_inputImage[i] = static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
    }
}

void ImageScaler::scaleBilinear(int outputWidth, int outputHeight) 
{
    // Escalar imagen en escala de grises usando la nueva API (STBIR_GREY)
    unsigned char *output_pixels = new unsigned char[outputWidth * outputHeight];
    stbir_resize_uint8_linear(
        m_inputImage.data(), m_width, m_height, 0,
        output_pixels, outputWidth, outputHeight, 0,
        STBIR_1CHANNEL
    );
    m_outputImage.assign(output_pixels, output_pixels + outputWidth * outputHeight);
    delete[] output_pixels;
    std::cout << "Imagen convertida y escalada a gris " << outputWidth << "x" << outputHeight << std::endl;
}
