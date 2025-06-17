#include <vector>
#include <string>

#include <cmath>
#include "ImageWriter.h"
#include "ImageReader.h"
#include "Image3DProjector.h"

int main()
{
	std::cout << "Starting the program" << std::endl;
	std::string lidarDataFile = "Scripts/LidarSample_1.txt";
	//"C:\\Users\\aayala01\\Documents\\Code\\miscelanea\\scripts-lidar-point-projections\\2011_10_03\\2011_10_03_drive_0047_sync\\velodyne_points\\data\\0000000336.bin";
	
	const auto lidarData = ImageReader().readLidarData(lidarDataFile);

	auto image3dProjector = Image3DProjector();
	const auto projectedPoints = image3dProjector.project3DImageTo2D(lidarData);
	
	//uint16_t width = 1240, height = 380;
	uint16_t width = 320, height = 240;

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