#pragma once

#include <array>
#include <cstdint>
#include <cstddef>

namespace jpeg{
    // Constants for DCT
    inline constexpr size_t DCT_BLOCK_SIZE = 8;
    inline constexpr size_t DCT_BLOCK_ELEMS = DCT_BLOCK_SIZE * DCT_BLOCK_SIZE;

    // Aliases for DCT blocks and coefficients
    using DCTBlock = std::array<uint8_t, DCT_BLOCK_ELEMS>;
    using DCTCoeffs = std::array<int16_t, DCT_BLOCK_ELEMS>;
    using DCTCosineTable = std::array<double, DCT_BLOCK_ELEMS>;
    using DCTCoeffsQuantizationTable = std::array<uint16_t, DCT_BLOCK_ELEMS>;
}