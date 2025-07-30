#include <iostream>
#include "jpeg/dct.hpp"
#include "jpeg/quantization.hpp"

int main() {
    jpeg::DCTBlock block = {
        32, 29, 27, 28, 28, 27, 28, 30,  // 第1行
        31, 30, 30, 31, 32, 30, 28, 28,  // 第2行
        30, 29, 30, 32, 33, 32, 29, 28,  // 第3行
        29, 27, 27, 28, 29, 30, 30, 30,  // 第4行
        9, 9, 6, 6, 14, 15, 12, 12,      // 第5行
        165, 169, 172, 171, 168, 166, 165, 163,  // 第6行
        156, 163, 167, 166, 163, 160, 157, 155,  // 第7行
        151, 158, 162, 161, 157, 156, 155, 155   // 第8行（根据用户描述实际有8行）
    };
    jpeg::DCTCoeffs coeffs = jpeg::transforms::dctInBlock(block);
    std::cout << "DCT Coefficients:" << std::endl;
    // 8x8 DCT系数输出
    for (int i = 0; i < jpeg::DCT_BLOCK_SIZE; i++) {
        for (int j = 0; j < jpeg::DCT_BLOCK_SIZE; j++) {
            std::cout << coeffs[i * jpeg::DCT_BLOCK_SIZE + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "DCT calculation completed." << std::endl;

    jpeg::quantization::quantizeInBlock(coeffs, jpeg::quantization::LUMINANCE_QUANTIZATION_TABLE);
    for(int i = 0; i < jpeg::DCT_BLOCK_SIZE; i++) {
        for (int j = 0; j < jpeg::DCT_BLOCK_SIZE; j++) {
            std::cout << coeffs[i * jpeg::DCT_BLOCK_SIZE + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Quantization completed." << std::endl;

    return 0;
}
