#include "Watermark.h"

bool applyWatermark(ImageBMP& image, const ImageBMP& watermark, double alpha) {
    if (alpha < 0.0 || alpha > 1.0 ||
        image.width() <= 0 || image.height() <= 0 ||
        watermark.width() <= 0 || watermark.height() <= 0 ||
        watermark.width() > image.width() ||
        watermark.height() > image.height()) {
        return false;
    }

    int startX = (image.width() - watermark.width()) / 2;
    int startY = (image.height() - watermark.height()) / 2;

    for (int y = 0; y < watermark.height(); ++y) {
        for (int x = 0; x < watermark.width(); ++x) {
            uint8_t* original = image.pixel(startX + x, startY + y);
            const uint8_t* mark = watermark.pixel(x, y);

            for (int color = 0; color < 3; ++color) {
                original[color] = static_cast<uint8_t>(
                    original[color] * (1.0 - alpha) + mark[color] * alpha
                );
            }
        }
    }

    return true;
}
