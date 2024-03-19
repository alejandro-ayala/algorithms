#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <cstdint>
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <cmath>
#include "external/stb/stb_image_write.h"
#include <algorithm>

namespace
{
    double findMax(const std::vector<std::vector<double>> &gradient)
    {
        // Inicializar el máximo con el valor mínimo posible
        double maxElement = std::numeric_limits<double>::lowest();

        // Recorrer cada fila de la matriz
        for (const auto &row : gradient)
        {
            // Recorrer cada elemento de la fila
            for (double element : row)
            {
                // Actualizar el máximo si el elemento actual es mayor
                if (element > maxElement)
                {
                    maxElement = element;
                }
            }
        }

        // Retornar el máximo encontrado
        return maxElement;
    }

    double findMin(const std::vector<std::vector<double>> &gradient)
    {
        // Inicializar el mínimo con el valor máximo posible
        double minElement = std::numeric_limits<double>::max();

        // Recorrer cada fila de la matriz
        for (const auto &row : gradient)
        {
            // Recorrer cada elemento de la fila
            for (double element : row)
            {
                // Actualizar el mínimo si el elemento actual es menor
                if (element < minElement)
                {
                    minElement = element;
                }
            }
        }

        // Retornar el mínimo encontrado
        return minElement;
    }
}
class FileHandler
{
private:
    /* data */
public:
    FileHandler(/* args */) = default;
    ~FileHandler() = default;

    std::vector<std::vector<uint8_t>> loadImage(const char *fileName);
    void writeImage(std::vector<std::vector<double>> image, const char *fileName);
};

std::vector<std::vector<uint8_t>> FileHandler::loadImage(const char *filePath)
{
    int width, height, channels;
    const uint8_t desiredChannels = 1;
    unsigned char *image = stbi_load(filePath, &width, &height, &channels, desiredChannels);
    std::cout << "stbi_load. Size: " << std::to_string(width) << "x" << std::to_string(height) << "-- channels: " << std::to_string(channels) << std::endl;

    std::vector<std::vector<uint8_t>> rawImage;
    rawImage.reserve(height);
    if (image != nullptr)
    {
        const auto dataSize = width * height;
        std::cout << "Image load correctly. Size: " << std::to_string(dataSize) << std::endl;
        std::vector<uint8_t> row(width);
        std::cout << "m_rawImageSize: " << std::to_string(rawImage.size()) << std::endl;
        std::cout << "rowSize: " << std::to_string(row.size()) << std::endl;

        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                // Calcular el índice en el arreglo unidimensional
                int index = i * width + j;
                // Asignar el valor del píxel a la matriz bidimensional
                row[j] = static_cast<uint8_t>(image[index]);
            }
            rawImage.push_back(row);
        }
        std::cout << "Mapper image to array: " << std::to_string(rawImage.size()) << " rows of " << std::to_string(rawImage.at(0).size()) << " elements" << std::endl;

        stbi_image_free(image);
    }
    else
    {
        std::cerr << "Error al cargar la imagen." << std::endl;
    }
    return rawImage;
}

void FileHandler::writeImage(std::vector<std::vector<double>> image, const char *fileName)
{
    int width = image[0].size();
    int height = image.size();

    // Crear un buffer de píxeles para la imagen en escala de grises
    std::vector<unsigned char> grayImage(width * height);

    // Escalar los valores del gradiente al rango [0, 255]
    double maxVal = findMax(image);
    double minVal = findMin(image);
    std::cout << "MaxValue: " << std::to_string(maxVal) << std::endl;
    std::cout << "MinValue: " << std::to_string(minVal) << std::endl;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            // Escalar el valor del gradiente al rango [0, 255]
            double scaledValue = 255 * (image[i][j] - minVal) / (maxVal - minVal);
            grayImage[i * width + j] = static_cast<unsigned char>(scaledValue);
        }
    }

    // Guardar la imagen en escala de grises usando STB Image
    stbi_write_png(fileName, width, height, 1, grayImage.data(), width);
}
