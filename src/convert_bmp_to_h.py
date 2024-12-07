from PIL import Image
import os

def bmp_to_rgb565(file_path):
    img = Image.open(file_path).convert("RGB")
    width, height = img.size
    output = []

    for y in range(height):
        for x in range(width):
            r, g, b = img.getpixel((x, y))
            rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
            output.append(f"0x{rgb565:04X}")

    return output, width, height

def save_as_c_array(file_path, output_array, width, height):
    base_name = os.path.splitext(os.path.basename(file_path))[0]
    c_array_name = f"frame_{base_name}"
    with open(f"{c_array_name}.h", "w") as f:
        f.write(f"const uint16_t {c_array_name}[{width * height}] = {{\n")
        for i, value in enumerate(output_array):
            f.write(value + (", " if (i + 1) % 12 != 0 else ",\n"))
        f.write("\n};\n")

if __name__ == "__main__":
    input_folder = "frames"  # Folder containing BMP files
    for file_name in os.listdir(input_folder):
        if file_name.endswith("034.bmp"):
            file_path = os.path.join(input_folder, file_name)
            print(f"Converting {file_name}...")
            output_array, width, height = bmp_to_rgb565(file_path)
            save_as_c_array(file_path, output_array, width, height)
    print("Conversion completed!")
