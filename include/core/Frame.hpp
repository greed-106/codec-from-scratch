#pragma once

#include <vector>

class Frame{
public:
    std::vector<uint8_t> data;
    int width = 0;
    int height = 0;
};