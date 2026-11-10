#include <iostream>
#include <string>

#include "ImageBMP.h"
#include "Watermark.h"

namespace {
const std::string kOriginalFilename = "original.bmp";
const std::string kWatermarkFilename = "watermark.bmp";
const std::string kResultFilename = "result.bmp";
const std::string kOversizedFilename = "oversized.bmp";

const double kTransparencyCoef = 0.3;

const int kLoadingErrorCode = 1;
const int kApplyWatermarkErrorCode = 2;
const int kSavingErrorCode = 3;
const int kResultFileSizeMismatchErrorCode = 4;
const int kBadTestsPassErrorCode = 5;

const int kResultWidth = 5;
const int kResultHeight = 5;

}  // namespace

int main() {
    ImageBMP image, mark;
    if (!image.load(kOriginalFilename)) {
        std::cerr << "Error loading " << kOriginalFilename << '\n';
        return kLoadingErrorCode;
    }
    if (!mark.load(kWatermarkFilename)) {
        std::cerr << "Error loading " << kWatermarkFilename << '\n';
        return kLoadingErrorCode;
    }

    if (!applyWatermark(image, mark, kTransparencyCoef)) {
        std::cerr << "Error while applying watermark\n";
        return kApplyWatermarkErrorCode;
    }

    if (!image.save(kResultFilename)) {
        std::cerr << "Couldn't save image to " << kResultFilename << '\n';
        return kSavingErrorCode;
    }

    ImageBMP result;
    if (!result.load(kResultFilename)) {
        std::cerr << "Error loading " << kResultFilename << '\n';
    }
    if (result.width() != kResultWidth || result.height() != kResultHeight) {
        std::cerr << "Result file size mismatch\n";
        return kResultFileSizeMismatchErrorCode;
    }
    for (int y = 0; y < kResultHeight; ++y) {
        for (int x = 0; x < kResultWidth; ++x) {
            const uint8_t* p = result.pixel(x, y);

            bool inside = x >= 1 && x <= 3 && y >= 1 && y <= 3;
            for (int c = 0; c < 3; ++c) {
                if (p[c] != (inside ? 170 : 200)) {
                    std::cerr << "Error occurred. Pixels mismatch: color: " << c
                              << " x: " << x << " y: " << y << '\n';
                    std::cerr << "p[c]: " << static_cast<int>(p[c]) << '\n';
                    return 6;
                }
            }
        }
    }
    ImageBMP oversized;
    if (!oversized.load(kOversizedFilename)) {
        std::cerr << "Error while loading " << kOversizedFilename << '\n';
        return kLoadingErrorCode;
    }
    if (applyWatermark(image, oversized, kTransparencyCoef)) {
        std::cerr << "ERROR: Oversized watermark successful\n";
        return kBadTestsPassErrorCode;
    }

    const double invalid_alpha_coef = -0.1;
    if (applyWatermark(image, mark, invalid_alpha_coef)) {
        std::cerr << "ERROR: wrond alpha coefficient run successful\n";
        return kBadTestsPassErrorCode;
    }

    std::cout << "All watermark tests passed\n";
    return 0;
}
