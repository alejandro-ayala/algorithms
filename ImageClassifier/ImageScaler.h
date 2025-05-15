
#include <cstdint>
#include <string>
#include <vector>
class ImageScaler
{   
public:
    ImageScaler() = default;
    ~ImageScaler() = default;

    explicit ImageScaler(const std::vector<uint8_t>& input, uint16_t width, uint16_t height, uint8_t channels);
    
    bool loadImage(const std::string& filename);
    void saveImage(const std::string& filename);
    void scaleBilinear(int outputWidth, int outputHeight);
    void rgb2Gray();
private:
    std::vector<uint8_t> m_inputImage;
    std::vector<uint8_t> m_outputImage;
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;

};