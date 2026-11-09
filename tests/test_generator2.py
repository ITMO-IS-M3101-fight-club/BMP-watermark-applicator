import math
import struct


def save_bmp(path, width, height, pixel):
    """Сохраняет 24-битный BMP. pixel(x, y) возвращает (R, G, B)."""
    row_size = (width * 3 + 3) & ~3
    pixel_data_size = row_size * height
    file_size = 54 + pixel_data_size

    with open(path, "wb") as f:
        f.write(b"BM")
        f.write(struct.pack("<IHHI", file_size, 0, 0, 54))
        f.write(struct.pack(
            "<IiiHHIIiiII",
            40, width, height, 1, 24, 0,
            pixel_data_size, 2835, 2835, 0, 0
        ))

        padding = b"\x00" * (row_size - width * 3)

        # BMP хранит строки снизу вверх
        for y in range(height - 1, -1, -1):
            row = bytearray()
            for x in range(width):
                r, g, b = pixel(x, y)
                row.extend((b & 255, g & 255, r & 255))
            f.write(row)
            f.write(padding)


def original_pixel(x, y):
    # Цветной градиент с шахматным узором и диагональными полосами
    checker = ((x // 6) + (y // 6)) % 2
    stripe = (x + 2 * y) % 19 < 5
    return (
        (x * 5 + y * 2 + checker * 45) % 256,
        (y * 7 + (stripe * 90)) % 256,
        (x * 3 + y * 5 + checker * 70) % 256,
    )


def watermark_pixel(x, y, width, height):
    # Цветной круглый знак с яркой рамкой и узором внутри
    cx, cy = (width - 1) / 2, (height - 1) / 2
    radius = min(width, height) * 0.46
    distance = math.hypot(x - cx, y - cy)

    if distance > radius:
        return (20, 30, 220)  # синий фон

    if distance > radius * 0.78:
        return (255, 220, 20)  # жёлтая рамка

    if (x // 3 + y // 3) % 2:
        return (245, 40, 170)  # розовый узор
    return (20, 230, 100)      # зелёный узор


# Три пары разных размеров — все водяные знаки меньше оригиналов.
cases = [
    ("test1", 64, 48, 19, 15),
    ("test2", 91, 67, 27, 21),
    ("test3", 128, 96, 35, 29),
]

for name, ow, oh, ww, wh in cases:
    save_bmp(f"{name}_original.bmp", ow, oh, original_pixel)
    save_bmp(
        f"{name}_watermark.bmp",
        ww,
        wh,
        lambda x, y, w=ww, h=wh: watermark_pixel(x, y, w, h),
    )

print("Созданы 6 BMP-файлов.")
