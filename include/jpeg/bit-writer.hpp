#pragma once

#include <cstdint>
#include <ostream>
#include <algorithm>
#include <iostream>

namespace jpeg {
    class BitWriter {
    private:
        std::ostream& outputStream;
        uint8_t buffer = 0;
        int bitsLengthInBuffer = 0;
        int bufferSize = sizeof(buffer) * 8; // Size of buffer in bits
    
        // write the data in buffer to the output stream
        void writeBuffer() {
            outputStream.put(buffer);
            // Marker Stuffing: If the byte is 0xFF, we need to write an extra 0x00 byte
            if(buffer == 0xFF) {
                outputStream.put(0x00);
            }
            buffer = 0; // Reset buffer after writing
            bitsLengthInBuffer = 0; // Reset bit length after writing

            if(!outputStream.good()) {
                throw std::ios_base::failure("Failed to write to output stream");
            }
        }
    public:
        BitWriter(std::ostream& stream) : outputStream(stream), buffer(0), bitsLengthInBuffer(0) {}
        // Disable copy constructor and assignment operator to prevent multiple states for the same writer
        BitWriter(const BitWriter&) = delete;
        BitWriter& operator=(const BitWriter&) = delete;
        void flush(){
            if(bitsLengthInBuffer > 0){
                // padding the remaining bits with 1
                buffer <<= (bufferSize - bitsLengthInBuffer);
                buffer |= (1 << (bufferSize - bitsLengthInBuffer)) - 1; // Set remaining bits to 1
                writeBuffer();
            }
        }

        ~BitWriter() {
            try{
                flush(); // Ensure all bits are written before destruction
            } catch (const std::ios_base::failure& e) {
                // Handle the error if needed, e.g., log it
                std::cerr << "Error during flushing BitWriter: " << e.what() << std::endl;
            }
        }
        /**
         * @brief 写入value的低bitsLength位到输出流
         * @param value 要写入的值
         * @param bitsLength 要写入的位数
         * @throws std::out_of_range 如果bitsLength不在1到32位之间
         * @throws std::ios_base::failure 如果写入输出流失败
         */
        void writeBits(uint32_t value, int bitsLength) {
            constexpr static int maxBitsLength = sizeof(value) * 8;
            if(bitsLength == 0) return; // No bits to write
            if(bitsLength < 0 || bitsLength > maxBitsLength) {
                throw std::out_of_range("Bits length must be between 1 and " + std::to_string(maxBitsLength));
            }
            
            while(bitsLength > 0) {
                // Calculate how many bits we can write in this iteration
                int bitsToWrite = std::min(bitsLength, bufferSize - bitsLengthInBuffer);
                // Shift the buffer to the left to make space for new bits
                buffer <<= bitsToWrite;
                // Add the new bits to the buffer
                buffer |= (value >> (bitsLength - bitsToWrite)) & ((1 << bitsToWrite) - 1);
                bitsLengthInBuffer += bitsToWrite;
                bitsLength -= bitsToWrite;
                // clear the bits that have been written
                value &= ((1u << bitsLength) - 1); // Keep only the remaining bits
                
                if(bitsLengthInBuffer == bufferSize) {
                    writeBuffer(); // Write the buffer if it's full
                }
            }
        }
    };
}