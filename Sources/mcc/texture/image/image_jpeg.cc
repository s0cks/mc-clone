#include "mcc/texture/image/image.h"

#include <cstdio>
#include <cstdlib>
#include <jerror.h>
#include <jpeglib.h>

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

  ImagePtr JpegImageDecoder::DecodeJPEG(FILE* file) {
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
    return Image::New(target(), type, size, data);
  }

  rx::observable<ImagePtr> JpegImageDecoder::Decode() {
    MCC_ASSERT(target().HasScheme("file"));
    MCC_ASSERT(target().HasExtension(".jpeg") || target().HasExtension(".jpg"));
    return rx::observable<>::create<ImagePtr>([this](rx::subscriber<ImagePtr> s) {
      const auto file = target().OpenFileForReading();
      if(!file) {
        const auto err = fmt::format("failed to open file: {0:s}", (const std::string&) target_);
        s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
        return;
      }

      const auto image = DecodeJPEG(file);
      if(!image) {
        const auto err = fmt::format("failed to decode PNG from file: {0:s}", (const std::string&) target_);
        s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
        return;
      }

      fclose(file);

      s.on_next(image);
      s.on_completed();
    });
  }
}