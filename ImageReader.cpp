#include "ImageReader.h"

std::vector<std::vector<float>> ImageReader::readLidarData(std::string filename)
{
    // Create an input file stream object named 'file' and
    // open the file "GFG.txt".
    bool isBinaryFile = false;
    if(endsWith(filename, ".bin"))
    {
        std::ifstream file(filename, std::ios::binary);
        isBinaryFile = true;
    }

	std::vector<std::vector<float>> lidarData;
    std::string line;
    if(isBinaryFile)
    {
        std::ifstream file(filename, std::ios::binary);
    
        if (!file) {
            std::cerr << "Unable to open file!" << std::endl;
            return std::vector<std::vector<float>>();
        }
    
        char byte;
    
    LidarPoint lidarPoint;
    
    while (file.read(reinterpret_cast<char*>(&lidarPoint), sizeof(LidarPoint))) 
    {
        //std::cout << "X: " << lidarPoint.x << ", Y: " << lidarPoint.y << ", Z: " << lidarPoint.z << std::endl;
        std::vector<float>lidarPointVector{lidarPoint.x, lidarPoint.y, lidarPoint.z};
        lidarData.push_back(lidarPointVector);
    }
        
        //std::cout << std::dec << std::endl; // Vuelve a formato decimal
        file.close();
    }
    else
    {
        std::ifstream file(filename);
        if (file.is_open()) {
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string s;
                std::vector<float>v;
                while (std::getline(ss, s, ' ')) 
                {
                    float num = std::stof(s);
                    v.push_back(num);
                }
                lidarData.push_back(v);		
            }
            file.close();
        }
        else {
            std::cerr << "Unable to open file!" << std::endl;
        }        

    }

	std::cout << "Readed Lidar sample size: " << lidarData.size() << std::endl;
	return lidarData;
}