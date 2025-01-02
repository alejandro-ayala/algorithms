#pragma once 

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include <fstream>

struct Color 
{
    uint8_t r, g, b;
};

class ImageWriter
{
private:
    uint16_t m_height;
    uint16_t m_width;
    std::string m_filename;
    Color getColor(float distance);
    void writeBMP(const std::string& filename, int width, int height, const std::vector<Color>& image);

public:
    ImageWriter(uint16_t width, uint16_t height, const std::string& fileName);
    ~ImageWriter() = default;
    void createBMPImage(const std::vector<std::vector<float>>& points);
};