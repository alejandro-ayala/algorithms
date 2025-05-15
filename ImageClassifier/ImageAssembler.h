#ifdef INC_ASSEMBLER
#pragma once 
#include <vector>
#include <cstdint>
class ImageAssembler
{
private:
    /* data */
public:
    ImageAssembler();
    ~ImageAssembler();

    bool loadData(const std::vector<uint8_t>& input);
    bool assembleImage();
    bool assembleFrame(uint8_t msgIndex, uint8_t cborIndex);
};

#endif