#include "ImageReader.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>


std::vector<std::vector<float>> ImageReader::readLidarData()
{
    // Create an input file stream object named 'file' and
    // open the file "GFG.txt".
    std::ifstream file("imageSamples/LidarSample_1.txt");
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

std::vector<std::vector<float>> ImageReader::readLidarData(const std::string& fileName, float& maxDist) {
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);  // abre en modo binario
    maxDist = std::numeric_limits<float>::lowest();
    std::vector<std::vector<float>> lidarData;

    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo: " << fileName << std::endl;
        return lidarData;
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize % 16 != 0) {
        std::cerr << "El tamaño del archivo no es múltiplo de 16 bytes (puntos mal formateados)." << std::endl;
        return lidarData;
    }

    size_t numPoints = fileSize / 16;
    std::vector<float> buffer(numPoints * 4);  // x, y, z, reflectance

    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        std::cerr << "Error al leer los datos binarios." << std::endl;
        return lidarData;
    }

    for (size_t i = 0; i < numPoints; ++i) {
        float x = buffer[4 * i];
        float y = buffer[4 * i + 1];
        float z = buffer[4 * i + 2];
        float r = buffer[4 * i + 3];  // reflectancia

        float dist = std::sqrt(x * x + y * y + z * z);
        if (dist > maxDist) {
            maxDist = dist;
        }        

        lidarData.push_back({x, y, z});  // como en Python: solo xyz
    }

    std::cout << "Leídos " << lidarData.size() << " puntos LIDAR." << std::endl;

    // Opcional: imprime los primeros 5 puntos para comparar
    for (int i = 0; i < 5; ++i) {
        std::cout << std::fixed << std::setprecision(3)
                  << "Lidar point: " << lidarData[i][0] << ", "
                  << lidarData[i][1] << ", " << lidarData[i][2] << std::endl;
    }

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