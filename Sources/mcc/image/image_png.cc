#include "mcc/image/image.h"

#include <png.h>
#include <string>
#include <glog/logging.h>

#include "mcc/image/image_data.h"

namespace mcc::img::png {
  ImagePtr PngImageDecoder::DecodePNG(FILE* file) {
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) {
      LOG(ERROR) << "failed to load png texture from: " << target();
      fclose(file);
      return nullptr;
    }

    png_infop info = png_create_info_struct(png);
    if(!info) {
      LOG(ERROR) << "failed to load png texture from: " << target();
      png_destroy_read_struct(&png, NULL, NULL);
      fclose(file);
      return nullptr;
    }

    if(setjmp(png_jmpbuf(png))) {
      LOG(ERROR) << "failed to load png texture from: " << target();
      png_destroy_read_struct(&png, &info, NULL);
      fclose(file);
      return nullptr;
    }

    png_init_io(png, file);
    png_set_sig_bytes(png, 0);
    png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    ImageType type;

    png_uint_32 width, height;
    int bit_depth, color_type;
    png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
    switch(color_type) {
      case PNG_COLOR_TYPE_RGBA:
        type = ImageType::kRGBA;
        break;
      case PNG_COLOR_TYPE_RGB:
      case PNG_COLOR_TYPE_GRAY:
        type = ImageType::kRGB;
        break;
      default:
        LOG(ERROR) << "unknown color type: " << color_type;
        fclose(file);
        return nullptr;
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
    const auto data = ImageData::New(total_size);
    png_bytepp rows = png_get_rows(png, info);
    for(auto i = 0; i < height; i++) {
      memcpy(data->bytes() + (row_bytes * (height - 1 - i)), rows[i], row_bytes);
    }
    png_destroy_read_struct(&png, &info, NULL);
    fclose(file);
    return Image::New(target(), type, ImageSize(width, height), data);
  }

  rx::observable<ImagePtr> PngImageDecoder::Decode() {
    return rx::observable<>::create<ImagePtr>([this](rx::subscriber<ImagePtr> s) {
      const auto file = target().OpenFileForReading();
      if(!file) {
        const auto err = fmt::format("failed to open file: {0:s}", (const std::string&) target_);
        s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
        return;
      }

      const auto image = DecodePNG(file);
      if(!image) {
        const auto err = fmt::format("failed to decode PNG from file: {0:s}", (const std::string&) target_);
        s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
        return;
      }
      s.on_next(image);
      s.on_completed();
    });
  }
}