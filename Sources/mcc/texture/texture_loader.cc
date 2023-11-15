#include "mcc/texture/texture_loader.h"
#include <cstdlib>
#include <cstdio>
#include "mcc/texture/image.h"

namespace mcc::texture {
  Texture PngTextureLoader::Load() {
    DLOG(INFO) << "loading " << filename_ << "....";
    Image image;
    if(!png::Decode(filename_, image)) {
      LOG(ERROR) << "failed to load " << filename_;
      return kInvalidTextureId;
    }

    DLOG(INFO) << "type: " << image.type;

    Texture texture(true);
    texture.Bind0();
    CHECK_GL(FATAL);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    CHECK_GL(FATAL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.size[0], image.size[1], 0, image.type, GL_UNSIGNED_BYTE, (GLvoid*) image.data->data());
    CHECK_GL(FATAL);
    glGenerateMipmap(GL_TEXTURE_2D);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    CHECK_GL(FATAL);
    return texture;
  }

  Texture JpegTextureLoader::Load() {
    DLOG(INFO) << "loading " << filename_ << "....";
    Image image;
    if(!jpeg::Decode(filename_, image)) {
      LOG(ERROR) << "failed to load " << filename_;
      return kInvalidTextureId;
    }

    Texture texture(true);
    texture.Bind0();
    CHECK_GL(FATAL);
    glTexImage2D(GL_TEXTURE_2D, 0, image.type, image.size[0], image.size[1], 0, image.type, GL_UNSIGNED_BYTE, (const GLvoid*) image.data->data());
    CHECK_GL(FATAL);
    glGenerateMipmap(GL_TEXTURE_2D);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    CHECK_GL(FATAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CHECK_GL(FATAL);
    return texture;
  }
}