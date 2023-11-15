#include "mcc/texture/image.h"
#include <jerror.h>
#include <jpeglib.h>

namespace mcc::jpeg {
  static inline Image::Type
  GetType(const int channels) {
    switch(channels) {
      case 4:
        return Image::kRGBA;
      case 3:
      default:
        return Image::kRGB;
    }
  }

  bool Decode(const std::string& filename, Image& result) {
    auto file = fopen(filename.c_str(), "rb");
    if(!file)
      return false;
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
    result.size[0] = info.output_width;
    result.size[1] = info.output_height;
    result.type = GetType(channels);
    const auto size = result.size[0] * result.size[1] * channels;
    const auto buffer = Buffer::New(size);
    while (info.output_scanline < info.output_height) {
      const auto row_ptr = &buffer->data()[channels * info.output_width * info.output_scanline];
      jpeg_read_scanlines(&info, (JSAMPARRAY)&row_ptr, 1);
    }
    result.data = buffer;
    jpeg_finish_decompress(&info);
    jpeg_destroy_decompress(&info);
    fclose(file);
    return true;
  }
}