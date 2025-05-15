#include "ImageWriter.h"

ImageWriter::ImageWriter(uint16_t width, uint16_t height, const std::string& fileName) : m_width(width), m_height(height), m_filename(fileName)
{}

Color ImageWriter::getColor(float distance) 
{
    if (distance < 50) return {255, 0, 0}; // R
    else if (distance < 100) return {0, 255, 0}; //G
    else return {0, 0, 255}; //B
}

void ImageWriter::createBMPImage(const std::vector<std::vector<float>>& points) 
{
    std::cout << "createBMPImage: " << m_filename << std::endl;
    std::vector<Color> image(m_width * m_height, {0, 0, 0}); // Inicializar con negro

    for (const auto& point : points) {
        int x = static_cast<int>(point[0]);
        int y = static_cast<int>(point[1]);
        float distance = point[2];

        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            image[y * m_width + x] = getColor(distance);
        }
    }

    writeBMP(m_filename, m_width, m_height, image);
}

void ImageWriter::writeBMP(const std::string& filename, int width, int height, const std::vector<Color>& image) {

    try
    {
        std::ofstream file(filename, std::ios::binary);

        // BMP Header
        uint32_t fileSize = 54 + width * height * 3;
        uint32_t reserved = 0;
        uint32_t offset = 54;
        uint32_t headerSize = 40;
        uint16_t planes = 1;
        uint16_t bitsPerPixel = 24;
        uint32_t compression = 0;
        uint32_t imageSize = width * height * 3;
        uint32_t xPixelsPerMeter = 0;
        uint32_t yPixelsPerMeter = 0;
        uint32_t totalColors = 0;
        uint32_t importantColors = 0;

        file.put('B').put('M');
        file.write(reinterpret_cast<char*>(&fileSize), 4);
        file.write(reinterpret_cast<char*>(&reserved), 4);
        file.write(reinterpret_cast<char*>(&offset), 4);
        file.write(reinterpret_cast<char*>(&headerSize), 4);
        file.write(reinterpret_cast<char*>(&width), 4);
        file.write(reinterpret_cast<char*>(&height), 4);
        file.write(reinterpret_cast<char*>(&planes), 2);
        file.write(reinterpret_cast<char*>(&bitsPerPixel), 2);
        file.write(reinterpret_cast<char*>(&compression), 4);
        file.write(reinterpret_cast<char*>(&imageSize), 4);
        file.write(reinterpret_cast<char*>(&xPixelsPerMeter), 4);
        file.write(reinterpret_cast<char*>(&yPixelsPerMeter), 4);
        file.write(reinterpret_cast<char*>(&totalColors), 4);
        file.write(reinterpret_cast<char*>(&importantColors), 4);

        // BMP Data
        for (int y = height - 1; y >= 0; --y) {
            for (int x = 0; x < width; ++x) {
                const Color& color = image[y * width + x];
                file.put(color.b).put(color.g).put(color.r);
            }
        }

        file.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}