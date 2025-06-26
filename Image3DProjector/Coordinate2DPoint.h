#pragma once 

struct Coordinate2DPoint
{
	Coordinate2DPoint() = default;
	Coordinate2DPoint(float xCoord, float yCoord) : x(xCoord), y(yCoord) {};
	float x;
	float y;
};
