#include "CartesianLidarPoint.h"


CartesianLidarPoint::CartesianLidarPoint(uint16_t distance, uint8_t angleH, uint8_t angleV)
{
    double theta = angleH * PI / 180.0; // azimut en radianes
    double phi = angleV * PI / 180.0;   // elevación en radianes

    xCoord = distance * std::cos(phi) * std::cos(theta);
    yCoord = distance * std::cos(phi) * std::sin(theta);
    zCoord = distance * std::sin(phi);
}

bool CartesianLidarPoint::operator==(const CartesianLidarPoint& other) const
{
    return  xCoord == other.xCoord &&
            yCoord == other.yCoord &&
            zCoord == other.zCoord;
}