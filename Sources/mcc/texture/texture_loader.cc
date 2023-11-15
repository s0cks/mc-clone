#include "mcc/texture/texture_loader.h"
#include <cstdlib>
#include <cstdio>
#include <png.h>
#include <jerror.h>
#include <jpeglib.h>

namespace mcc::texture {
  Texture PngTextureLoader::Load() {
    auto file = fopen(filename_.c_str(), "rb");
    if(!file) {
      LOG(FATAL) << "failed to open: " << filename_;
      return kInvalidTextureId;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) {
      fclose(file);
      LOG(FATAL) << "failed to load png texture from: " << filename_;
      return kInvalidTextureId;
    }
 
    png_infop info = png_create_info_struct(png);
    if(!info) {
      png_destroy_read_struct(&png, NULL, NULL);
      fclose(file);
      LOG(FATAL) << "failed to load png texture from: " << filename_;
      return kInvalidTextureId;
    }

    if(setjmp(png_jmpbuf(png))) {
      png_destroy_read_struct(&png, &info, NULL);
      fclose(file);
      LOG(FATAL) << "failed to load png texture from: " << filename_;
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
      LOG(FATAL) << "failed to load png texture from file: " << filename_;
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

    Texture texture(true);
    texture.Bind0();
    CHECK_GL(FATAL);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    CHECK_GL(FATAL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, alpha, GL_UNSIGNED_BYTE, (GLvoid*) data);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CHECK_GL(FATAL);
    png_destroy_read_struct(&png, &info, NULL);
    free(data);
    fclose(file);
    return texture;
  }

  Texture JpegTextureLoader::Load() {
    DLOG(INFO) << "loading " << filename_ << "....";
    unsigned long x, y;
    unsigned long data_size;     // length of the file
    int channels;               //  3 =>RGB   4 =>RGBA 
    unsigned int type;  
    unsigned char * rowptr[1];    // pointer to an array
    unsigned char * jdata;        // data for the image
    struct jpeg_decompress_struct info; //for our jpeg info
    struct jpeg_error_mgr err;          //the error handler

    FILE* file = fopen(filename_.c_str(), "rb");  //open the file

    info.err = jpeg_std_error(& err);     
    jpeg_create_decompress(& info);   //fills info structure

    //if the jpeg file doesn't load
    if(!file) {
      fprintf(stderr, "Error reading JPEG file %s!", filename_.c_str());
      return 0;
    }

    jpeg_stdio_src(&info, file);    
    jpeg_read_header(&info, TRUE);   // read jpeg file header

    jpeg_start_decompress(&info);    // decompress the file

    //set width and height
    x = info.output_width;
    y = info.output_height;
    channels = info.num_components;
    type = GL_COMPRESSED_RGB;
    if(channels == 4) type = GL_COMPRESSED_RGBA;

    data_size = x * y * 3;

    //--------------------------------------------
    // read scanlines one at a time & put bytes 
    //    in jdata[] array. Assumes an RGB image
    //--------------------------------------------
    jdata = (unsigned char *)malloc(data_size);
    while (info.output_scanline < info.output_height) // loop
    {
      // Enable jpeg_read_scanlines() to fill our jdata array
      rowptr[0] = (unsigned char *)jdata +  // secret to method
              3* info.output_width * info.output_scanline; 

      jpeg_read_scanlines(&info, rowptr, 1);
    }
    //---------------------------------------------------

    jpeg_finish_decompress(&info);   //finish decompressing

    //----- create OpenGL tex map (omit if not needed) --------
    Texture texture(true);
    texture.Bind0();
    CHECK_GL(FATAL);
    glTexImage2D(GL_TEXTURE_2D, 0, type, x, y, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) jdata);
    CHECK_GL(FATAL);
    // glGenerateMipmap(GL_TEXTURE_2D);
    // CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CHECK_GL(FATAL);

    jpeg_destroy_decompress(&info);
    fclose(file);                    //close the file
    free(jdata);
    return texture;    // for OpenGL tex maps
  }
}