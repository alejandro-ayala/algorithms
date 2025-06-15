#include "ImageReader.h"

#include <fstream>
#include <vector>
#include <iostream>

std::vector<std::vector<float>> ImageReader::readLidarData(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary); // modo binario

    std::vector<std::vector<float>> lidarData;

    if (!file) {
        std::cerr << "Unable to open file: " << fileName << std::endl;
        return lidarData;
    }

    // Leer el archivo completo en bloques de 4 floats
    float point[4];
    while (file.read(reinterpret_cast<char*>(point), sizeof(point))) {

        std::cout << "Point: x=" << point[0]
                  << ", y=" << point[1]
                  << ", z=" << point[2]
                  << ", r=" << point[3] << std::endl;
        lidarData.push_back({point[0], point[1], point[2], point[3]});
    }

    std::cout << "Read " << lidarData.size() << " LIDAR points from " << fileName << std::endl;
    return lidarData;
}

/*
std::vector<std::vector<float>> ImageReader::readLidarData(std::string fileName)
{
    // Create an input file stream object named 'file' and
    // open the file "GFG.txt".
    std::ifstream file(fileName);
	std::vector<std::vector<float>> lidarData;
    // String to store each line of the file.
    std::string line;

    if (file.is_open()) {
        // Read each line from the file and store it in the
        // 'line' variable.
        while (std::getline(file, line)) {
            //std::cout << line << std::endl;
			std::stringstream ss(line);
			std::string s;
			std::vector<float>v;
			while (std::getline(ss, s, ' ')) 
			{
				float num = std::stof(s);
				//std::cout << "value: " << num << std::endl;
				v.push_back(num);
			}
			lidarData.push_back(v);		
        }

        // Close the file stream once all lines have been
        // read.
        file.close();
    }
    else {
        // Print an error message to the standard error
        // stream if the file cannot be opened.
        std::cerr << "Unable to open file!" << std::endl;
    }

	std::cout << "Readed Lidar sample size: " << lidarData.size() << std::endl;
	return lidarData;
}

*/