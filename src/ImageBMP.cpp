#include "ImageBMP.h"
#include <fstream>
#include <iostream>
#include <cmath>

bool ImageBMP::load(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) { std::cerr << "Cannot open " << filename << "\n"; return false; }

    in.read(reinterpret_cast<char*>(&fileHeader_), sizeof(fileHeader_));
    in.read(reinterpret_cast<char*>(&infoHeader_), sizeof(infoHeader_));

    if (fileHeader_.bfType != 0x4D42) { std::cerr << "Not a BMP\n"; return false; }
    if (infoHeader_.biBitCount != 24 || infoHeader_.biCompression != 0) {
        std::cerr << "Only 24-bit uncompressed BMP supported\n"; return false;
    }

    const int w = infoHeader_.biWidth;
    const int h = std::abs(infoHeader_.biHeight);
    const int rowSize = ((w * 3 + 3) / 4) * 4;

    std::vector<uint8_t> row(rowSize);
    pixels_.assign(w * h * 3, 0);

    for (int fileRow = 0; fileRow < h; ++fileRow) {
        in.read(reinterpret_cast<char*>(row.data()), rowSize);
        int y = (infoHeader_.biHeight > 0) ? (h - 1 - fileRow) : fileRow;
        for (int x = 0; x < w; ++x) {
            pixels_[(y * w + x) * 3 + 0] = row[x * 3 + 0];
            pixels_[(y * w + x) * 3 + 1] = row[x * 3 + 1];
            pixels_[(y * w + x) * 3 + 2] = row[x * 3 + 2];
        }
    }
    return true;
}

bool ImageBMP::save(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out) { std::cerr << "Cannot write " << filename << "\n"; return false; }

    const int w = infoHeader_.biWidth;
    const int h = std::abs(infoHeader_.biHeight);
    const int rowSize = ((w * 3 + 3) / 4) * 4;
    const uint32_t imageSize = rowSize * h;

    BMPFileHeader fh = fileHeader_;
    BMPInfoHeader ih = infoHeader_;
    fh.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
    fh.bfSize    = fh.bfOffBits + imageSize;
    ih.biSizeImage = imageSize;

    out.write(reinterpret_cast<const char*>(&fh), sizeof(fh));
    out.write(reinterpret_cast<const char*>(&ih), sizeof(ih));

    std::vector<uint8_t> row(rowSize, 0);
    for (int fileRow = 0; fileRow < h; ++fileRow) {
        int y = (infoHeader_.biHeight > 0) ? (h - 1 - fileRow) : fileRow;
        for (int x = 0; x < w; ++x) {
            row[x * 3 + 0] = pixels_[(y * w + x) * 3 + 0];
            row[x * 3 + 1] = pixels_[(y * w + x) * 3 + 1];
            row[x * 3 + 2] = pixels_[(y * w + x) * 3 + 2];
        }
        out.write(reinterpret_cast<const char*>(row.data()), rowSize);
    }
    return true;
}

uint8_t* ImageBMP::pixel(int x, int y) {
    return &pixels_[(y * infoHeader_.biWidth + x) * 3];
}

const uint8_t* ImageBMP::pixel(int x, int y) const {
    return &pixels_[(y * infoHeader_.biWidth + x) * 3];
}