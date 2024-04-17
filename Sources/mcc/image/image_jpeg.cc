#include "mcc/image/image.h"

#include <cstdio>
#include <cstdlib>
#include <jerror.h>
#include <jpeglib.h>
#include <fmt/format.h>

namespace mcc::img::jpeg {
  static inline ImageType
  GetType(const int channels) {
    switch(channels) {
      case 4:
        return ImageType::kRGBA;
      case 3:
      default:
        return ImageType::kRGB;
    }
  }

  Image* Decode(FILE* file) {
    MCC_ASSERT(file);

    ImageType type;
    ImageSize size;

    unsigned long data_size;
    int channels;
    unsigned char* jdata;
    struct jpeg_decompress_struct info;
    struct jpeg_error_mgr err;
    info.err = jpeg_std_error(&err);     
    jpeg_create_decompress(&info);
    jpeg_stdio_src(&info, file);    
    jpeg_read_header(&info, TRUE);
    jpeg_start_decompress(&info);
    channels = info.num_components;
    size[0] = info.output_width;
    size[1] = info.output_height;
    type = GetType(channels);
    const auto total_size = size[0] * size[1] * channels;
    const auto data = ImageData::New(total_size);
    while (info.output_scanline < info.output_height) {
      const auto row_ptr = &data->bytes()[channels * info.output_width * info.output_scanline];
      jpeg_read_scanlines(&info, (JSAMPARRAY)&row_ptr, 1);
    }
    jpeg_finish_decompress(&info);
    jpeg_destroy_decompress(&info);
    return Image::New(type, size, data);
  }

  rx::observable<Image*> DecodeAsync(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("file"));
    MCC_ASSERT(uri.HasExtension(".jpeg") || uri.HasExtension(".jpg"));
    return rx::observable<>::create<Image*>([uri](rx::subscriber<Image*> s) {
      const auto file = uri.OpenFileForReading();
      if(!file) {
        const auto err = fmt::format("failed to open file: {0:s}", (const std::string&) uri);
        s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
        return;
      }

      const auto image = Decode(file);
      if(!image) {
        const auto err = fmt::format("failed to decode JPEG from file: {0:s}", (const std::string&) uri);
        s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
        return;
      }
      s.on_next(image);
      s.on_completed();
    });
  }
}