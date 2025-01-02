#include <vector>
#include <string>

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