
#define IMAGE3D_PROJECTOR

#include <string>
#include <iostream>
#ifdef IMAGE3D_PROJECTOR
#include <vector>


#include <cmath>
#include "Image3DProjector/ImageWriter.h"
#include "Image3DProjector/ImageReader.h"
#include "Image3DProjector/Image3DProjector.h"

using Point = std::vector<float>;
float safe(const std::vector<std::vector<float>>& img, int y, int x);
std::vector<std::vector<float>> createDepthImage(const std::vector<Point>& points, int width, int height);
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
std::vector<std::vector<float>> computeSobelMagnitude(const std::vector<std::vector<float>>& depthImg) {
    int height = depthImg.size();
    int width = depthImg[0].size();
    std::vector<std::vector<float>> gradImg(height, std::vector<float>(width, 0.0f));

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float gx =
                -safe(depthImg, y-1, x-1) + safe(depthImg, y-1, x+1)
                -2 * safe(depthImg, y, x-1) + 2 * safe(depthImg, y, x+1)
                -safe(depthImg, y+1, x-1) + safe(depthImg, y+1, x+1);

            float gy =
                -safe(depthImg, y-1, x-1) - 2 * safe(depthImg, y-1, x) - safe(depthImg, y-1, x+1)
                +safe(depthImg, y+1, x-1) + 2 * safe(depthImg, y+1, x) + safe(depthImg, y+1, x+1);

            gradImg[y][x] = std::sqrt(gx * gx + gy * gy);
        }
    }

    return gradImg;
}

void normalizeToGrayscale(
    const std::vector<std::vector<float>>& input,
    std::vector<uint8_t>& output,
    int& width,
    int& height
) {
    height = input.size();
    width = input[0].size();
    output.resize(width * height);

    float maxVal = 0.0f;
    for (const auto& row : input)
        for (float v : row)
            if (v > maxVal) maxVal = v;

    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x) {
            float norm = input[y][x] / (maxVal + 1e-5f);
            output[y * width + x] = static_cast<uint8_t>(norm * 255.0f);
        }
}

// Paso 4: Convertir a RGB (escala de grises)
void grayscaleToRGB(
    const std::vector<uint8_t>& gray,
    std::vector<uint8_t>& rgb,
    int width,
    int height
) {
    rgb.resize(width * height * 3);
    for (int i = 0; i < width * height; ++i) {
        rgb[3 * i + 0] = gray[i];
        rgb[3 * i + 1] = gray[i];
        rgb[3 * i + 2] = gray[i];
    }
}

std::vector<std::vector<float>> createDepthImage(const std::vector<Point>& points, int width, int height) {
    std::vector<std::vector<float>> depthImg(height, std::vector<float>(width, -1.0f));

    for (const auto& point : points) {
        int x = static_cast<int>(point[0]);
        int y = static_cast<int>(point[1]);
        float d = point[2];

        if (x >= 0 && x < width && y >= 0 && y < height) {
            if (depthImg[y][x] < 0.0f || d < depthImg[y][x])
                depthImg[y][x] = d;
        }
    }

    return depthImg;
}

float safe(const std::vector<std::vector<float>>& img, int y, int x) {
    if (y < 0 || x < 0 || y >= img.size() || x >= img[0].size()) return 0.0f;
    float val = img[y][x];
    return (val < 0.0f) ? 0.0f : val;
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
/*
    std::vector<std::string> inputFiles;
    uint8_t lidarSamples = 15;
    for(int i = 1; i <= lidarSamples; i++)
    {
        std::string fileName = "lidar" + std::to_string(i);
        inputFiles.push_back(fileName);
    }
    
       //inputFiles.push_back("lidar15");
*/
    //std::vector<std::string> inputFiles ={{"lidar2"},{"lidar3"},{"lidar5"},{"lidar6"},{"lidar7"},{"lidar8"},{"lidar9"},{"lidar11"},{"lidar12"},{"lidar13"},{"lidar14"},{"lidar15"}};
    std::vector<std::string> inputFiles ={{"calibrateFile"}};
#endif
    for(const auto& inputFileName : inputFiles)
    {
#ifdef VELODYNE_DATA
        const std::string basePath = "Resources\\Lidar-Camera-Calibration\\Velodyne\\";
        const std::string fullFilename = basePath + inputFileName + fileFormat;
	    const auto lidarData = ImageReader().readLidarData(fullFilename, maxDistance);
#else
        const std::string basePath = "Resources\\Lidar-Camera-Calibration\\GarminLite\\";
        const std::string fileFormat = ".txt";        
        const std::string fullFilename = basePath + inputFileName + fileFormat;
	    const auto lidarData = ImageReader().readLidarDataFromTxtFile(fullFilename, maxDistance);
#endif
        std::cout << "Readed Lidar sample size: " << lidarData.size() << std::endl;
        std::cout << "maxDistance: " << std::to_string(maxDistance) << std::endl;

        auto image3dProjector = Image3DProjector();
        std::vector<std::vector<float>> projectedImg = image3dProjector.project3DImageTo2D(lidarData);

        std::string basePathOutput = basePath + "output\\";
        std::string outputFilename = basePathOutput+ inputFileName + "_projected" + ".bmp";
        ImageWriter(cameraResolutionWidth, cameraResolutionHeight, outputFilename).createBMPImage(projectedImg, maxDistance);

        auto depthImg = createDepthImage(projectedImg, cameraResolutionWidth, cameraResolutionHeight);
        auto sobelImg   = computeSobelMagnitude(depthImg);

        std::vector<uint8_t> grayImg;
        int w, h;
        normalizeToGrayscale(sobelImg , grayImg, w, h);

        std::vector<uint8_t> rgbImg;
        grayscaleToRGB(grayImg, rgbImg, w, h);
        std::string processedOutputFilename = basePathOutput + inputFileName + "_sobel" + ".png";
        ImageWriter().saveImageRGB(processedOutputFilename, rgbImg, w, h);
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