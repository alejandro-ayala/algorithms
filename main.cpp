
#define IMAGE3D_PROJECTOR

#include <string>
#include <iostream>
#ifdef IMAGE3D_PROJECTOR
#include <vector>


#include <cmath>
#include "Image3DProjector/ImageWriter.h"
#include "Image3DProjector/ImageReader.h"
#include "Image3DProjector/Image3DProjector.h"

#ifdef VELODYNE_DATA
	uint16_t cameraResolutionWidth = 1240, cameraResolutionHeight = 380;
#else
    uint16_t cameraResolutionWidth = 320, cameraResolutionHeight = 240;
#endif
void disableLogOutput()
{
    std::streambuf* original_cout_buffer = std::cout.rdbuf();
    std::ofstream null_stream("/dev/null"); 
    std::cout.rdbuf(null_stream.rdbuf());
}
int main()
{
    

	std::cout << "Starting the program" << std::endl;
    float maxDistance = 0.0;

    const std::string fileFormat = ".bin";
#ifdef VELODYNE_DATA  
    disableLogOutput();
    std::vector<std::string> inputFiles ={{"000007"},{"0000000107"},{"0000000001"}};
#else
    std::vector<std::string> inputFiles ={{"LidarSample_1"}};
#endif
    for(const auto& inputFileName : inputFiles)
    {
#ifdef VELODYNE_DATA
        const std::string basePath = "Resources\\Lidar-Camera-Calibration\\Velodyne\\";
        const std::string fullFilename = basePath + inputFileName + fileFormat;
	    const auto lidarData = ImageReader().readLidarData(fullFilename, maxDistance);
#else
        const std::string basePath = "Resources\\Lidar-Camera-Calibration\\Velodyne\\";
        const std::string fileFormat = ".txt";        
        const std::string fullFilename = basePath + inputFileName + fileFormat;
	    const auto lidarData = ImageReader().readLidarDataFromTxtFile(fullFilename);
#endif
        std::cout << "Readed Lidar sample size: " << lidarData.size() << std::endl;
        std::cout << "maxDistance: " << std::to_string(maxDistance) << std::endl;

        auto image3dProjector = Image3DProjector();
        std::vector<std::vector<float>> projectedImg = image3dProjector.project3DImageTo2D(lidarData);
        
        std::string outputFilename = basePath + inputFileName + "_projected" + ".bmp";
        ImageWriter(cameraResolutionWidth, cameraResolutionHeight, outputFilename).createBMPImage(projectedImg, maxDistance);
    }
    
    //return 0;
}

#else
#include <iostream>
#include <filesystem>
#include <string>
#include "ImageClassifier/ImageScaler.h"

namespace fs = std::filesystem;

int main() {
    const std::string input_folder = "Resources/imageSamples";
	std::string output_folder = input_folder + "/out";
	fs::create_directories(output_folder);
    const int target_width = 96;
    const int target_height = 96;
    std::cout << "Starting programm" << std::endl;
    ImageScaler imgScaler;

    for (const auto& entry : fs::directory_iterator(input_folder)) {
        if (entry.is_regular_file()) {
            auto path = entry.path();
            if (path.extension() == ".jpeg" || path.extension() == ".jpg") {
                std::cout << "Processing: " << path.string() << std::endl;

                int width, height, channels;
                auto input_image = imgScaler.loadImage(path.string());

                if (!input_image) {
                    std::cerr << "Failed to load image: " << path.string() << std::endl;
                    continue;
                }

                // Convert to gray
                imgScaler.rgb2Gray();

                // Scale
                unsigned char* output_gray = new unsigned char[target_width * target_height];
                imgScaler.scaleBilinear(target_width, target_height);

                // Output file name
				std::string output_filename = output_folder + "/" + path.stem().string() + "_gray_" + std::to_string(target_width) + "x" + std::to_string(target_height) + ".raw";
                imgScaler.saveImage(output_filename);

                std::cout << "Saved: " << output_filename << std::endl;
/*
                delete[] gray_image;
                delete[] output_gray;
                stbi_image_free(input_image);
*/
            }
        }
    }

    std::cout << "All images processed." << std::endl;

	bool convertToCarray = true;
	if(convertToCarray)
	{
		std::cout << "Launching script to generate C arrays..." << std::endl;
		int result = system("python Resources/scripts/convertToCarray.py");
		if (result != 0) {
			std::cerr << "Error running Python script." << std::endl;
		} else {
			std::cout << "C arrays generated successfully." << std::endl;
		}		
	}
    return 0;
}

#endif