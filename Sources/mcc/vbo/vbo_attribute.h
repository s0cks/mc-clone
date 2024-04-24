#ifndef MCC_VBO_ATTRIBUTE_H
#define MCC_VBO_ATTRIBUTE_H

#include "mcc/gfx.h"

namespace mcc::vbo {
  typedef uint64_t AttributeOffset;
  typedef uint64_t AttributeStride;
  typedef uint32_t AttributeIndex;
  typedef uint32_t AttributeSize;

  template<const AttributeIndex Index,
           const AttributeSize Size,
           const GLenum Type,
           const GLboolean Normalized,
           const AttributeStride Stride,
           const AttributeOffset Offset>
  class Attribute {
  public:
    static inline constexpr AttributeIndex GetIndex() {
      return Index;
    }

    static inline constexpr AttributeSize GetSize() {
      return Size;
    }

    static inline constexpr GLenum GetType() {
      return Type;
    }

    static inline constexpr GLboolean IsNormalized() {
      return Normalized;
    }

    static inline constexpr AttributeStride GetStride() {
      return Stride;
    }

    static inline constexpr AttributeOffset GetOffset() {
      return Offset;
    }

    static inline void
    Enable() {
      glEnableVertexAttribArray(GetIndex());
      CHECK_GL(FATAL);
    }

    static inline void
    Bind(const GLvoid* ptr) {
      glVertexAttribPointer(GetIndex(), GetSize(), GetType(), IsNormalized(), GetStride(), ptr);
      CHECK_GL(FATAL);
    }

    static inline void
    Bind() {
      return Bind((const GLvoid*) GetOffset());
    }

    static inline void
    BindAndEnable(const GLvoid* ptr) {
      Bind(ptr);
      Enable();
    }

    static inline void
    BindAndEnable() {
      Bind();
      Enable();
    }
  };

  template<const AttributeIndex Index, 
           const AttributeOffset Offset,
           const AttributeStride Stride>
  class Vec2fAttribute : public Attribute<Index, 2, GL_FLOAT, GL_FALSE, Stride, Offset>{};

  template<const AttributeIndex Index,
           const AttributeOffset Offset,
           const AttributeStride Stride>
  class Vec3fAttribute : public Attribute<Index, 3, GL_FLOAT, GL_FALSE, Stride, Offset>{};

  template<const AttributeIndex Index,
           const AttributeOffset Offset,
           const AttributeStride Stride>
  class ColorAttribute : public Attribute<Index, 4, GL_UNSIGNED_BYTE, GL_FALSE, Stride, Offset>{};
}

#endif //MCC_VBO_ATTRIBUTE_H