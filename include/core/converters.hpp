#pragma once

#include "frames.hpp"
#include <algorithm>
#include <cmath>

namespace converters {

    uint8_t clampToUint8(double value){
        return static_cast<uint8_t>(std::clamp(static_cast<int>(std::round(value)), 0, 255));
    }

    // RGB to YCbCr with 4:2:0 subsampling
    YCbCrFrame rgbToYCbCr420(const RGBFrame& rgbFrame) {
        YCbCrFrame ycbcrFrame;

        // Set dimensions
        ycbcrFrame.yWidth = rgbFrame.width;
        ycbcrFrame.yHeight = rgbFrame.height;
        ycbcrFrame.cbcrWidth = (rgbFrame.width + 1) / 2; // 4:2:0 subsampling
        ycbcrFrame.cbcrHeight = (rgbFrame.height + 1) / 2;

        // Resize vectors
        ycbcrFrame.yData.resize(ycbcrFrame.yWidth * ycbcrFrame.yHeight);
        ycbcrFrame.cbData.resize(ycbcrFrame.cbcrWidth * ycbcrFrame.cbcrHeight);
        ycbcrFrame.crData.resize(ycbcrFrame.cbcrWidth * ycbcrFrame.cbcrHeight);

        // Convert RGB to YCbCr
        for (uint32_t y = 0; y < rgbFrame.height; y++){
            for(uint32_t x = 0; x < rgbFrame.width; x++){
                uint32_t index = y * rgbFrame.width + x;
                uint8_t r = rgbFrame.rgbData[index * 3];
                uint8_t g = rgbFrame.rgbData[index * 3 + 1];
                uint8_t b = rgbFrame.rgbData[index * 3 + 2];

                // YCbCr conversion formulas
                uint8_t yValue = clampToUint8(0.299 * r + 0.587 * g + 0.114 * b);
                // uint8_t cbValue = clampToUint8(-0.168736 * r - 0.331364 * g + 0.5 * b + 128);
                // uint8_t crValue = clampToUint8(0.5 * r - 0.418688 * g - 0.081312 * b + 128);

                // Store Y value
                ycbcrFrame.yData[index] = yValue;

                // Store Cb and Cr values in 4:2:0 format
                // Caculate the average for Cb and Cr in 2x2 blocks
                if (x % 2 == 0 && y % 2 == 0){
                    uint32_t cbcrIndex = (y / 2) * ycbcrFrame.cbcrWidth + (x / 2);

                    double cbSum = 0.0;
                    double crSum = 0.0;
                    // Count the number of valid pixels in the 2x2 block
                    int count = 0;

                    for (int dy = 0; dy < 2; dy++) {
                        for (int dx = 0; dx < 2; dx++) {
                            int pixelX = x + dx;
                            int pixelY = y + dy;
                            // Check bounds
                            if (pixelX < rgbFrame.width && pixelY < rgbFrame.height) {
                                uint32_t pixelIndex = pixelY * rgbFrame.width + pixelX;
                                uint8_t rBlock = rgbFrame.rgbData[pixelIndex * 3];
                                uint8_t gBlock = rgbFrame.rgbData[pixelIndex * 3 + 1];
                                uint8_t bBlock = rgbFrame.rgbData[pixelIndex * 3 + 2];

                                // Accumulate Cb and Cr values
                                cbSum += -0.168736 * rBlock - 0.331364 * gBlock + 0.5 * bBlock + 128;
                                crSum += 0.5 * rBlock - 0.418688 * gBlock - 0.081312 * bBlock + 128;
                                count++;
                            }
                        }
                    }

                    // Calculate the average Cb and Cr values
                    ycbcrFrame.cbData[cbcrIndex] = clampToUint8(cbSum / count);
                    ycbcrFrame.crData[cbcrIndex] = clampToUint8(crSum / count);
                }
            }
        }

        return ycbcrFrame;
    }
};