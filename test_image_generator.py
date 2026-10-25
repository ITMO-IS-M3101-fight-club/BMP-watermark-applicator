import struct

def create_test_bmp(filename):
    w, h = 2, 2
    row_size = 8 
    
    top_row = bytes([0, 0, 255,  0, 255, 0]) + b'\x00\x00'
    bottom_row = bytes([255, 0, 0,  255, 255, 255]) + b'\x00\x00'

    pixel_data = bottom_row + top_row
    
    image_size = len(pixel_data)
    file_size = 54 + image_size 
    file_header = struct.pack('<2sIHHI', b'BM', file_size, 0, 0, 54)
    info_header = struct.pack('<IiiHHIIiiII', 40, w, h, 1, 24, 0, image_size, 2835, 2835, 0, 0)
    
    with open(filename, 'wb') as f:
        f.write(file_header)
        f.write(info_header)
        f.write(pixel_data)

if __name__ == "__main__":
    create_test_bmp("tests/test_2x2.bmp")