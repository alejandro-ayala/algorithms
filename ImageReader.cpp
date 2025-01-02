#include "ImageReader.h"

std::vector<std::vector<float>> ImageReader::readLidarData()
{
    // Create an input file stream object named 'file' and
    // open the file "GFG.txt".
    std::ifstream file("imageSamples/lidarSample.txt");
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