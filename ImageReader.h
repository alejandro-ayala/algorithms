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
    std::vector<std::vector<float>> readLidarData(std::string filename = "imageSamples/lidarSample.txt");
    
struct LidarPoint {
     float x, y, z, intensidad;
    };

    static inline bool endsWith(const std::string& str, const std::string& sufijo) {
         return str.size() >= sufijo.size() &&
            str.compare(str.size() - sufijo.size(), sufijo.size(), sufijo) == 0;
        }
        
};