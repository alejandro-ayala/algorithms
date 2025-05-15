#ifdef INC_ASSEMBLER

#include "ImageAssembler.h"

ImageAssembler::ImageAssembler()
{
}

bool ImageAssembler::loadData(const std::vector<uint8_t>& input)
{
}

bool ImageAssembler::assembleImage()
{
}

bool ImageAssembler::assembleFrame(uint8_t msgIndex, uint8_t cborIndex)
{
    LOG_TRACE("CAMERA_IMAGE frame: ", std::to_string(msgIndex), " completed with ", std::to_string(cborIndex), " cbor msgs");
    const auto storedMsg = m_cameraFramesQueue->getStoredMsg();
    std::vector<uint8_t> cborFrame;
    if(cborIndex != storedMsg)
    {
        LOG_ERROR("CAMERA_IMAGE frame: ", std::to_string(msgIndex), " completed with ", std::to_string(cborIndex), " cbor msgs, but ", std::to_string(storedMsg), " stored msgs in queue");
    }

    cborFrame.reserve(storedMsg * hardware_abstraction::Controllers::CAN_DATA_PAYLOAD_SIZE);

    //TODO check if iterate upto cborIndex or storedMsgs
    for(size_t idx = 0; idx < storedMsg; idx++)
    {
        std::array<uint8_t, hardware_abstraction::Controllers::CAN_DATA_PAYLOAD_SIZE> rxBuffer;
        m_cameraFramesQueue->receive(rxBuffer.data());
        cborFrame.insert(cborFrame.end(), rxBuffer.begin() + 2, rxBuffer.end());
    }
    const auto pendingStoredMsg = m_cameraFramesQueue->getStoredMsg();
    if(pendingStoredMsg != 0)
        LOG_WARNING("CAMERA_IMAGE frame: pending frames after read queue: ", std::to_string(pendingStoredMsg));

    auto it = std::find_if(cborFrame.rbegin(), cborFrame.rend(), [](uint8_t val) { return val != 0xFF; });
    cborFrame.erase(it.base(), cborFrame.end());

    try
    {
        auto m_dataSerializer = std::make_shared<business_logic::DataSerializer>();
        business_logic::ImageSnapshot cborImgChunk;
        m_dataSerializer->deserialize(cborImgChunk, cborFrame);
        //TODO store the frame in the cborChunkQueue
        //m_cameraFramesQueue->send(cborImgChunk);
    }
    catch (const std::exception& e) 
    {
        LOG_ERROR("Exception caught: " + std::string(e.what()));
    }
    catch (...) 
    {
        LOG_ERROR("Unknown exception caught during frame deserialization.");
    }
    break;
}

#endif