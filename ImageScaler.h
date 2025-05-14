
#include <cstdint>
#include <string>
class ImageScaler
{   
public:
    ImageScaler() = default;
    ~ImageScaler() = default;
    
    unsigned char* loadImage(const std::string& filename, int &width, int &height, int &channels);
    void saveImage(const std::string& filename, unsigned char* outputBuffer, int bufferSize);
    void scaleBilinear(uint8_t *input, uint8_t *output, int input_width, int input_height, int output_width, int output_height);
    void rgb2Gray(const unsigned char* rgb, unsigned char* gray, int width, int height);
private:
    unsigned char* m_inputImage;
    unsigned char* m_outputImage;
};