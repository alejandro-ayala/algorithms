#include "CartesianLidarPoint.h"


CartesianLidarPoint::CartesianLidarPoint(float distance, float angleH, float angleV)
{

    float theta = angleH * PI / 180.0f; // azimut
    float phi   = angleV * PI / 180.0f; // elevación

    xCoord = distance * std::cos(phi) * std::cos(theta); // frente
    yCoord = distance * std::cos(phi) * std::sin(theta); // izquierda
    zCoord = distance * std::sin(phi);                   // arriba
}

bool CartesianLidarPoint::operator==(const CartesianLidarPoint& other) const
{
    return  xCoord == other.xCoord &&
            yCoord == other.yCoord &&
            zCoord == other.zCoord;
}