#pragma once 
#include <cstdint>

class IEdgeDetectorAlgorithm
{
public:
    virtual void process(uint8_t* image, uint8_t* detectedEged) = 0;
};

