#include "Image3DProjector.h"

std::vector<std::vector<float>> Image3DProjector::multiplyMatrix(const std::vector<std::vector<float>>& a, const std::vector<std::vector<float>>& b) const
{
	std::vector<std::vector<float>> result(a.size(), std::vector<float>(b.at(0).size(), 0.0f));
	for(size_t rowIdx = 0; rowIdx < a.size(); rowIdx++)
	{
		for(size_t columnIdx = 0; columnIdx < b.at(0).size(); columnIdx++)
		{
			for(size_t idx = 0; idx < b.size(); idx++)
			{
				result[rowIdx][columnIdx] += a[rowIdx][idx] * b[idx][columnIdx];
			}
		}
	}
	return result;
}

Coordinate3DPoint Image3DProjector::applyExtrinsicTransformation(const Coordinate3DPoint& lidarPoint)
{
	Coordinate3DPointHomogeneous lidarPointHomog(lidarPoint);
	std::vector<std::vector<float>>lidarCoordPoint{{{{lidarPointHomog.x}}, {{lidarPointHomog.y}}, {{lidarPointHomog.z}}, {{lidarPointHomog.w}}}};
	const auto camCoorPoint = multiplyMatrix(m_projectionConfig.m_cameraExtrinsicMatrix, lidarCoordPoint);
	return Coordinate3DPoint(camCoorPoint[0][0], camCoorPoint[1][0], camCoorPoint[2][0]);
}

Coordinate3DPoint Image3DProjector::applyIntrinsicProjection(const Coordinate3DPoint& camCoordPoint)
{
	std::vector<std::vector<float>>cameraCoordPoint{{{{camCoordPoint.x}}, {{camCoordPoint.y}}, {{camCoordPoint.z}}}};
	const auto projectedPoint = multiplyMatrix(m_projectionConfig.m_cameraIntrinsicMatrix, cameraCoordPoint);
	return Coordinate3DPoint(projectedPoint[0][0], projectedPoint[1][0], projectedPoint[2][0]);
}

Coordinate3DPoint Image3DProjector::project3DPointTo2D(const Coordinate3DPoint& lidarPoint)
{
	const auto transformedPoint = applyExtrinsicTransformation(lidarPoint);
	const auto projectedPoint   = applyIntrinsicProjection(transformedPoint);
	return Coordinate3DPoint(projectedPoint.x / projectedPoint.z, projectedPoint.y / projectedPoint.z, projectedPoint.z);
}

Coordinate2DPoint Image3DProjector::distortPoint(float x, float y)
{
	const float k1 = -0.48344743;
	const float k2 =  0.72221231;
	const float p1 =  0.00657751;
	const float p2 = -0.00430634;
	const float k3 = -1.1407779;

    float r2 = x * x + y * y;
    float radial = 1 + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2;

    float x_distorted = x * radial + 2 * p1 * x * y + p2 * (r2 + 2 * x * x);
    float y_distorted = y * radial + p1 * (r2 + 2 * y * y) + 2 * p2 * x * y;

    return Coordinate2DPoint{x_distorted, y_distorted};
}

Coordinate3DPoint Image3DProjector::project3DPointTo2DWithDistorsion(const Coordinate3DPoint& lidarPoint)
{
    const auto transformedPoint = applyExtrinsicTransformation(lidarPoint);
    float x = transformedPoint.x / transformedPoint.z;
    float y = transformedPoint.y / transformedPoint.z;
	auto distorted = distortPoint(x, y);
    float fx = m_projectionConfig.m_cameraIntrinsicMatrix[0][0];
    float fy = m_projectionConfig.m_cameraIntrinsicMatrix[1][1];
    float cx = m_projectionConfig.m_cameraIntrinsicMatrix[0][2];
    float cy = m_projectionConfig.m_cameraIntrinsicMatrix[1][2];

    float u = fx * distorted.x + cx;
    float v = fy * distorted.y + cy;

    return Coordinate3DPoint(u, v, transformedPoint.z);	

}

std::vector<std::vector<float>> Image3DProjector::project3DImageTo2D(const std::vector<std::vector<float>>& image3d)
{
    std::vector<std::vector<float>> projectedImg;
	for(const auto& point : image3d)
	{        
#ifdef VELODYNE_DATA
			uint8_t offsetH = 120;
			uint8_t offsetV = 130;
            Coordinate3DPoint lidarPoint(point[0], point[1], point[2]);
            const auto projectedPoint = project3DPointTo2D(lidarPoint);
            std::vector<float> projectedPointVector{projectedPoint.x, projectedPoint.y, projectedPoint.z};
            projectedImg.push_back(projectedPointVector);
#else
            auto cartesianLidarPoint = CartesianLidarPoint(point[2], point[0], point[1]);
            Coordinate3DPoint lidarPoint(cartesianLidarPoint.xCoord, cartesianLidarPoint.yCoord, cartesianLidarPoint.zCoord); 
            const auto projectedPoint = project3DPointTo2D(lidarPoint);
            std::vector<float> projectedPointVector{projectedPoint.x, projectedPoint.y, projectedPoint.z};
			std::cout << "Lidar point : " << point[0] << ", " << point[1] << ", " << point[2] << std::endl;
			std::cout << "Lidar point (cartesian): " << lidarPoint.x << ", " << lidarPoint.y << ", " << lidarPoint.z << std::endl;
			std::cout << "Projected point: " << projectedPointVector[0] << ", " << projectedPointVector[1] << ", " << projectedPointVector[2] << std::endl;
            const auto projectedPointDist = project3DPointTo2DWithDistorsion(lidarPoint);
			//std::cout << "Projected point with distorsion: " << projectedPointDist.x << ", " << projectedPointDist.y << ", " << projectedPointDist.z << std::endl;
			
			projectedImg.push_back(projectedPointVector);
 #endif
	}
    return projectedImg;
}