#include "ImageBMP.h"
#include <iostream>

int main() {
    std::cout << "--- Test: BMPImage 2x2 ---\n";
    ImageBMP img;

    if (!img.load("tests/test_2x2.bmp")) {
        std::cerr << "Cannot load the file" << '\n';
        return 1;
    }
    std::cout << "Loaded. Size: " << img.width() << "x" << img.height() << '\n';

    const uint8_t* p1 = img.pixel(0, 0);
    const uint8_t* p2 = img.pixel(1, 0);
    const uint8_t* p3 = img.pixel(0, 1);
    const uint8_t* p4 = img.pixel(1, 1);

    if ((p1[0] == 0 && p1[1] == 0 && p1[2] == 255) && (p2[0] == 0 && p2[1] == 255 && p2[2] == 0) && (p3[0] == 255 && p3[1] == 0 && p3[2] == 0) && (p4[0] == 255 && p4[1] == 255 && p4[2] == 255));
    std::cout << "Colors check completed" << '\n';

    if (!img.save("tests/test_2x2_copy.bmp")) {
        std::cerr << "Cannot save the file" << '\n';
        return 1;
    }

    return 0;
}