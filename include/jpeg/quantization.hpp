#pragma once

#include "dct-constants.hpp"
#include <cmath>
#include <algorithm>

namespace jpeg{

namespace quantization{

    // Standard Luminance Quantization Table
    inline constexpr DCTCoeffsQuantizationTable LUMINANCE_QUANTIZATION_TABLE = {
        16, 11, 10, 16, 24, 40, 51, 61,
        12, 12, 14, 19, 26, 58, 60, 55,
        14, 13, 16, 24, 40, 57, 69, 56,
        14, 17, 22, 29, 51, 87, 80, 62,
        18, 22, 37, 56, 68,109,103,77,
        24, 35,55 ,64 ,81 ,104 ,113 ,92,
        49 ,64 ,78 ,87 ,103 ,121 ,120 ,101,
        72 ,92 ,95 ,98 ,112 ,100 ,103 ,99
    };

    // Standard Chrominance Quantization Table
    inline constexpr DCTCoeffsQuantizationTable CHROMINANCE_QUANTIZATION_TABLE = {
        17, 18, 24, 47, 99, 99, 99, 99,
        18, 21, 26, 66, 99, 99, 99, 99,
        24, 26, 56, 99, 99, 99, 99, 99,
        47, 66, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99,
        99, 99, 99, 99, 99, 99, 99, 99
    };

    std::array<DCTCoeffsQuantizationTable, 2> scaleQuantizationTables(int quality){
        // Quality factor to scaling factor
        int factor = 0;
        if(quality < 50) {
            factor = 5000 / quality;
        } else {
            factor = 200 - quality * 2;
        }

        DCTCoeffsQuantizationTable luminanceTable = LUMINANCE_QUANTIZATION_TABLE;
        DCTCoeffsQuantizationTable chrominanceTable = CHROMINANCE_QUANTIZATION_TABLE;

        std::transform(luminanceTable.begin(), luminanceTable.end(), luminanceTable.begin(),
            [factor](uint16_t value) {
                return static_cast<uint16_t>(std::clamp(static_cast<int>(std::round(value * factor / 100.0)), 1, 255));
            });
    }

    void quantizeInBlock(DCTCoeffs& coeffs, const DCTCoeffsQuantizationTable& table) {
        std::transform(coeffs.begin(), coeffs.end(), table.begin(), coeffs.begin(),
            [](int16_t coeff, uint16_t quant) {
                return static_cast<int16_t>(std::round(static_cast<double>(coeff) / quant));
            });
    }
}

}