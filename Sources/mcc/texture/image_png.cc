#include "mcc/texture/image.h"
#include <png.h>

namespace mcc::png {
  bool Decode(const std::string& filename, Image& result) {
    auto file = fopen(filename.c_str(), "rb");
    if(!file) {
      LOG(ERROR) << "failed to open: " << filename;
      return false;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) {
      LOG(ERROR) << "failed to load png texture from: " << filename;
      fclose(file);
      return false;
    }

    png_infop info = png_create_info_struct(png);
    if(!info) {
      LOG(ERROR) << "failed to load png texture from: " << filename;
      png_destroy_read_struct(&png, NULL, NULL);
      fclose(file);
      return false;
    }

    if(setjmp(png_jmpbuf(png))) {
      LOG(ERROR) << "failed to load png texture from: " << filename;
      png_destroy_read_struct(&png, &info, NULL);
      fclose(file);
      return false;
    }

    png_init_io(png, file);
    png_set_sig_bytes(png, 0);
    png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    png_uint_32 width, height;
    int bit_depth, color_type;
    png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
    switch(color_type) {
      case PNG_COLOR_TYPE_RGBA:
        result.type = Image::kRGBA;
        break;
      case PNG_COLOR_TYPE_RGB:
      case PNG_COLOR_TYPE_GRAY:
        result.type = Image::kRGB;
        break;
      default:
        LOG(ERROR) << "unknown color type: " << color_type;
    }

    // if(bit_depth == 16)
    //   png_set_strip_16(png);
    // if(color_type == PNG_COLOR_TYPE_PALETTE)
    //   png_set_palette_to_rgb(png);
    // if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    //   png_set_expand_gray_1_2_4_to_8(png);
    // if(png_get_valid(png, info, PNG_INFO_tRNS))
    //   png_set_tRNS_to_alpha(png);
    // if(color_type == PNG_COLOR_TYPE_RGB ||
    //    color_type == PNG_COLOR_TYPE_GRAY ||
    //    color_type == PNG_COLOR_TYPE_PALETTE) {
    //   png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    // }
    // if(color_type == PNG_COLOR_TYPE_GRAY ||
    //    color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
    //   png_set_gray_to_rgb(png);
    // }

    const auto row_bytes = png_get_rowbytes(png, info);
    const auto total_size = row_bytes * height;
    auto buffer = Buffer::New(total_size);
    png_bytepp rows = png_get_rows(png, info);
    for(auto i = 0; i < height; i++) {
      memcpy(buffer->data() + (row_bytes * (height - 1 - i)), rows[i], row_bytes);
    }

    result.data = buffer;
    result.size[0] = width;
    result.size[1] = height;
    png_destroy_read_struct(&png, &info, NULL);
    fclose(file);
    return true;
  }
}