#pragma once

#include <vector>
#include <cstdint>

class RGBFrame{
public:
    // RGB数据
    std::vector<uint8_t> rgbData;
    uint32_t width = 0;
    uint32_t height = 0; 
};

class YCbCrFrame{
public:
    // YCbCr数据
    std::vector<uint8_t> yData;
    std::vector<uint8_t> cbData;
    std::vector<uint8_t> crData;
    uint32_t yWidth = 0;
    uint32_t yHeight = 0;
    uint32_t cbcrWidth = 0;
    uint32_t cbcrHeight = 0;
};