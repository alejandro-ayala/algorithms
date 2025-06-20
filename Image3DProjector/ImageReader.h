#pragma once 
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include <fstream>

class ImageReader
{
private:
    /* data */
public:
    ImageReader() = default;
    ~ImageReader() = default;
    std::vector<std::vector<float>> readLidarData(const std::string& fileName, float& maxDist);
    std::vector<std::vector<float>> readLidarDataFromTxtFile(const std::string& fileName);
};