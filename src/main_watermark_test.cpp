#include "ImageBMP.h"
#include "Watermark.h"
#include <iostream>

int main() {
    ImageBMP image, mark;
    if (!image.load("original.bmp") || !mark.load("watermark.bmp")) return 1;
    if (!applyWatermark(image, mark, 0.3)) return 2;
    if (!image.save("result.bmp")) return 3;
    ImageBMP result;
    if (!result.load("result.bmp")) return 4;
    if (result.width() != 5 || result.height() != 5) return 5;
    for (int y = 0; y < 5; ++y) for (int x = 0; x < 5; ++x) {
        const uint8_t* p = result.pixel(x,y);
        bool inside = x >= 1 && x <= 3 && y >= 1 && y <= 3;
        for (int c=0;c<3;++c) if (p[c] != (inside ? 170 : 200)) return 6;
    }
    ImageBMP oversized;
    if (!oversized.load("oversized.bmp")) return 7;
    if (applyWatermark(image, oversized, 0.3)) return 8;
    if (applyWatermark(image, mark, -0.1)) return 9;
    std::cout << "All watermark tests passed\n";
}
