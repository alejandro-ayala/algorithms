#pragma once 

#include <vector>
#include <cstdint>
//#define VELODYNE_DATA
extern uint16_t cameraResolutionWidth;
extern uint16_t cameraResolutionHeight;
struct Image3DProjectorConfig
{
#ifdef VELODYNE_DATA
	const float m_focalLenX = 718.856;
	const float m_opticalCenterX = 607.1928;
	const float m_focalLenY = 718.856;
	const float m_opticalCenterY = 185.2157;
	const std::vector<std::vector<float>> m_cameraIntrinsicMatrix{{
	    {m_focalLenX, 0.0, m_opticalCenterX},
	    {0.0, m_focalLenY, m_opticalCenterY},
	    {0.0, 0.0, 1.0}
	}};
	const std::vector<std::vector<float>> m_cameraExtrinsicMatrix{
    {0.0f, -1.0f,  0.0f, 0.0f},  // X_cam = -Y_lidar
    {0.0f,  0.0f, -1.0f, 0.0f},  // Y_cam = -Z_lidar
    {1.0f,  0.0f,  0.0f, 0.0f},  // Z_cam =  X_lidar
    {0.0f,  0.0f,  0.0f, 1.0f}
};
#else
	const float m_focalLenX = 367.61993872;
	const float m_opticalCenterX = 165.23052156;
	const float m_focalLenY = 365.71696573;
	const float m_opticalCenterY = 116.66181125;
	const std::vector<std::vector<float>> m_cameraIntrinsicMatrix{
    {m_focalLenX, 0.0f, m_opticalCenterX},
    {0.0f, m_focalLenY, m_opticalCenterY},
    {0.0f, 0.0f, 1.0f}
	};
	const std::vector<std::vector<float>> m_cameraExtrinsicMatrix{
    {0.0f, -1.0f,  0.0f, -13.5f},  // X_cam = -Y_lidar (lidar is desplaced by -9.5m in X)
    {0.0f,  0.0f, 1.0f, 1.5f},  // Y_cam = -Z_lidar
    {1.0f,  0.0f,  0.0f, 0.0f},  // Z_cam =  X_lidar
    {0.0f,  0.0f,  0.0f, 1.0f}
};
#endif
};