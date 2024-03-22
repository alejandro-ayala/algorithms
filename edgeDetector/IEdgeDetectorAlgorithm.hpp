#pragma once
#include <cstdint>
#include <vector>
class IEdgeDetectorAlgorithm
{
public:
    virtual std::vector<std::vector<double>> process(const std::vector<std::vector<uint8_t>> &image) = 0;
};
