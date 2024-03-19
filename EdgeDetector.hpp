#pragma once
#include "IEdgeDetectorAlgorithm.hpp"
#include "SobelEdgeDetectorAlgorithm.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include "EdgeDetector.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb/stb_image.h"

class EdgeDetector
{
private:
    std::shared_ptr<IEdgeDetectorAlgorithm> m_detectionEdgeAlgo;
    std::vector<unsigned char> m_rawImage;
    void loadImage(const char* filePath);
    void processImage();
    void printEdges();
public:
    EdgeDetector(const std::shared_ptr<IEdgeDetectorAlgorithm>& detectionEdgeAlgo = std::make_shared<SobelEdgeDetectorAlgorithm>());
    ~EdgeDetector();
};




EdgeDetector::EdgeDetector(const std::shared_ptr<IEdgeDetectorAlgorithm>& detectionEdgeAlgo) : m_detectionEdgeAlgo(detectionEdgeAlgo)
{
    loadImage("resources/test.jpg");
}

EdgeDetector::~EdgeDetector()
{
}

void EdgeDetector::loadImage(const char* filePath)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filePath, &width, &height, &channels, 0);
    

    if (image != nullptr) {
        const auto dataSize = width * height;
        std::cout << "Imagen cargada exitosamente." << std::endl;
        m_rawImage.reserve(dataSize);
        std::copy(image, image + dataSize, std::back_inserter(m_rawImage));

        stbi_image_free(image);
    } else {
        std::cerr << "Error al cargar la imagen." << std::endl;
    }
}

void EdgeDetector::processImage()
{
    const size_t size = m_rawImage.size(); 
    uint8_t* detected_edges;
    m_detectionEdgeAlgo->process(m_rawImage.data(), detected_edges);
}

void EdgeDetector::printEdges()
{
}