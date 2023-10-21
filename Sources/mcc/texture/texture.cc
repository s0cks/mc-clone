#include "mcc/texture/texture.h"
#include <cstdlib>
#include <cstdio>
#include <png.h>

namespace mcc::texture {
  static inline bool
  CheckHeader(FILE* file) {
    static png_byte header[8];
    fread(header, 1, 8, file);
    return png_sig_cmp(header, 0, 8) == 0;
  }

  Texture Texture::LoadFrom(const std::string& filename) {
    auto file = fopen(filename.c_str(), "rb");
    if(!file) {
      LOG(FATAL) << "failed to open: " << filename;
      return kInvalidTextureId;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) {
      fclose(file);
      LOG(FATAL) << "failed to load png texture from: " << filename;
      return kInvalidTextureId;
    }
 
    png_infop info = png_create_info_struct(png);
    if(!info) {
      png_destroy_read_struct(&png, NULL, NULL);
      fclose(file);
      LOG(FATAL) << "failed to load png texture from: " << filename;
      return kInvalidTextureId;
    }

    if(setjmp(png_jmpbuf(png))) {
      png_destroy_read_struct(&png, &info, NULL);
      fclose(file);
      LOG(FATAL) << "failed to load png texture from: " << filename;
      return kInvalidTextureId;
    }

    png_init_io(png, file);
    png_set_sig_bytes(png, 0);
    png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    png_uint_32 width, height;
    int bit_depth, color_type;
    png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
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

    const unsigned int rowbytes = png_get_rowbytes(png, info);
    GLubyte* data = (GLubyte*)malloc(rowbytes * height);
    if(!data) {
      png_destroy_read_struct(&png, &info, NULL);
      fclose(file);
      LOG(FATAL) << "failed to load png texture from file: " << filename;
      return kInvalidTextureId;
    }

    png_bytepp rows = png_get_rows(png, info);
    for(auto i = 0; i < height; i++) {
      memcpy(data+(rowbytes * (height - 1 - i)), rows[i], rowbytes);
    }

    GLint alpha;
    switch(png_get_color_type(png, info)) {
      case PNG_COLOR_TYPE_RGBA:
        alpha = GL_RGBA;
        break;
      case PNG_COLOR_TYPE_RGB:
        alpha = GL_RGB;
        break;
      default:
        LOG(FATAL) << "unknown color type.";
    }

    TextureId texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, alpha, GL_UNSIGNED_BYTE, (GLvoid*) data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    png_destroy_read_struct(&png, &info, NULL);
    free(data);
    fclose(file);
    return texture;
  }
}