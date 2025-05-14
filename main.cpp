
#include <string>
#include <iostream>
#ifdef IMAGE3D_PROJECTOR
#include <vector>


#include <cmath>
#include "ImageWriter.h"
#include "ImageReader.h"
#include "Image3DProjector.h"

int main()
{
	std::cout << "Starting the program" << std::endl;

	const auto lidarData = ImageReader().readLidarData();

	auto image3dProjector = Image3DProjector();
	const auto projectedPoints = image3dProjector.project3DImageTo2D(lidarData);
	
	uint16_t width = 1240, height = 380;
	ImageWriter(width, height, "output.bmp").createBMPImage(projectedPoints);
	std::cout << "createBMPImage done" << std::endl;	

	
	std::vector<std::vector<float>> projectedImg;
	for(const auto& point : lidarData)
	{
		
		Coordinate3DPoint lidarPoint(point[0], point[1], point[2]);
		const auto projectedPoint = image3dProjector.project3DPointTo2D(lidarPoint);
		std::vector<float> projectedPointVector{projectedPoint.x, projectedPoint.y, projectedPoint.z};
		projectedImg.push_back(projectedPointVector);
	}
	ImageWriter(width, height, "output2.bmp").createBMPImage(projectedPoints);
    return 0;
}

#else
#include <iostream>
#include <filesystem>
#include <string>
#include "ImageScaler.h"

namespace fs = std::filesystem;

int main() {
    const std::string input_folder = "Resources/imageSamples";
	std::string output_folder = input_folder + "/out";
	fs::create_directories(output_folder);
    const int target_width = 96;
    const int target_height = 96;

    ImageScaler imgScaler;

    for (const auto& entry : fs::directory_iterator(input_folder)) {
        if (entry.is_regular_file()) {
            auto path = entry.path();
            if (path.extension() == ".jpeg" || path.extension() == ".jpg") {
                std::cout << "Processing: " << path.string() << std::endl;

                int width, height, channels;
                auto input_image = imgScaler.loadImage(path.string(), width, height, channels);

                if (!input_image) {
                    std::cerr << "Failed to load image: " << path.string() << std::endl;
                    continue;
                }

                // Convert to gray
                unsigned char* gray_image = new unsigned char[width * height];
                imgScaler.rgb2Gray(input_image, gray_image, width, height);

                // Scale
                unsigned char* output_gray = new unsigned char[target_width * target_height];
                imgScaler.scaleBilinear(gray_image, output_gray, width, height, target_width, target_height);

                // Output file name
				std::string output_filename = output_folder + "/" + path.stem().string() + "_gray_" + std::to_string(target_width) + "x" + std::to_string(target_height) + ".raw";
                imgScaler.saveImage(output_filename, output_gray, target_width * target_height);

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