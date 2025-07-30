#pragma once

#include "dct-constants.hpp"
#include <array>
#include <cmath>
#include <algorithm>
#include <ranges>

namespace jpeg {

namespace transforms {
    

    /**
     * DCT_COS_TABLE[x * DCT_BLOCK_SIZE + u] = cos((2 * x + 1) * u * π / (2 * N))
     * N = DCT_BLOCK_SIZE = 8
     * x is the space index (0 to 7)
     * u is the frequency index (0 to 7)
     * This table is used to calculate the DCT coefficients for an 8x8 block
     */
    constexpr DCTCosineTable DCT_COSINE_TABLE = {
        1.000000,  0.980785,  0.923880,  0.831470,  0.707107,  0.555570,  0.382683,  0.195090,
        1.000000,  0.831470,  0.382683, -0.195090, -0.707107, -0.980785, -0.923880, -0.555570,
        1.000000,  0.555570, -0.382683, -0.980785, -0.707107,  0.195090,  0.923880,  0.831470,
        1.000000,  0.195090, -0.923880, -0.555570,  0.707107,  0.831470, -0.382683, -0.980785,
        1.000000, -0.195090, -0.923880,  0.555570,  0.707107, -0.831470, -0.382683,  0.980785,
        1.000000, -0.555570, -0.382683,  0.980785, -0.707107, -0.195090,  0.923880, -0.831470,
        1.000000, -0.831470,  0.382683,  0.195090, -0.707107,  0.980785, -0.923880,  0.555570,
        1.000000, -0.980785,  0.923880, -0.831470,  0.707107, -0.555570,  0.382683, -0.195090
    };



    // Function to perform DCT in an 8x8 block
    DCTCoeffs dctInBlock(const DCTBlock& block) {
        DCTCoeffs coeffs = {};

        // Level Shift
        // auto shfitedRange = block | std::views::transform([](uint8_t val) { return static_cast<int16_t>(val) - 128; });
        // std::vector<int16_t> shiftedBlock(shfitedRange.begin(), shfitedRange.end());
        std::array<int16_t, DCT_BLOCK_ELEMS> shiftedBlock;
        std::transform(block.begin(), block.end(), shiftedBlock.begin(),
            [](uint8_t val) { return static_cast<int16_t>(val) - 128; });

        std::array<double, DCT_BLOCK_ELEMS> tempCoefficents = {};

        // DCT Calculation
        // DCT in row direction
        // y is the row index in the DCT block
        for(int y = 0; y < DCT_BLOCK_SIZE; y++){
            // u is the frequency index
            for(int u = 0; u < DCT_BLOCK_SIZE; u++){
                double sum = 0.0;
                // x is the column index (spatial index) in the DCT block
                for(int x = 0; x < DCT_BLOCK_SIZE; x++){
                    sum += shiftedBlock[y * DCT_BLOCK_SIZE + x] * DCT_COSINE_TABLE[x * DCT_BLOCK_SIZE + u];
                }
                // Normalization factor = sqrt(2 / DCT_BLOCK_SIZE) for u != 0, and sqrt(1 / DCT_BLOCK_SIZE) for u == 0
                double alpha = (u == 0) ? 0.5 / 1.414213562 : 0.5;
                tempCoefficents[y * DCT_BLOCK_SIZE + u] = alpha * sum;
            }
        }
        // DCT in column direction
        // u is the column index in the DCT block
        for(int u = 0; u < DCT_BLOCK_SIZE; u++){
            // v is the frequency index
            for(int v = 0; v < DCT_BLOCK_SIZE; v++){
                double sum = 0.0;
                // y is the row index in the DCT block
                for(int y = 0; y < DCT_BLOCK_SIZE; y++){
                    sum += tempCoefficents[y * DCT_BLOCK_SIZE + u] * DCT_COSINE_TABLE[y * DCT_BLOCK_SIZE + v];
                }
                // Normalization factor = sqrt(2 / DCT_BLOCK_SIZE) for v != 0, and sqrt(1 / DCT_BLOCK_SIZE) for v == 0
                double alpha = (v == 0) ? 0.5 / 1.414213562 : 0.5;
                coeffs[v * DCT_BLOCK_SIZE + u] = static_cast<int16_t>(std::round(alpha * sum));
            }
        }

        return coeffs;
    }
}
    
}
