#ifndef MCC_TEXT_H
#define MCC_TEXT_H

#include <string>
#include <unordered_set>

#include "mcc/glm.h"
#include "mcc/object.h"
#include "mcc/font/font.h"

namespace mcc::text {
  using mcc::font::Font;

  class Text : public Object {
  public:
    struct Equals {
      bool operator()(const Text& lhs, const Text& rhs) const {
        const auto& lhs_value = lhs.GetValue();
        const auto& rhs_value = rhs.GetValue();
        return lhs_value.compare(rhs_value) == 0;
      }

      bool operator()(const Text* lhs, const Text* rhs) const {
        return operator()(*lhs, *rhs);
      }
    };

    struct Hash {
      size_t operator()(const Text& rhs) const {
        size_t hash;
        combine(hash, rhs.GetValue());
        return hash;
      }

      size_t operator()(const Text* rhs) const {
        MCC_ASSERT(rhs);
        return operator()(*rhs);
      }
    private:
      template<typename T>
      static inline void
      combine(size_t& hash, const T& value) {
        std::hash<T> hasher;
        hash ^= hasher(value) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
      }
    };
  protected:
    Font* font_;
    std::string value_;
    glm::vec2 pos_;
  public:
    Text(Font* font, const std::string& value, const glm::vec2& pos);
    ~Text() override;
    std::string ToString() const override;

    Font* GetFont() const {
      return font_;
    }

    const std::string& GetValue() const {
      return value_;
    }

    const glm::vec2& GetPos() const {
      return pos_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Text& rhs) {
      return stream << rhs.ToString();
    }
  };
  typedef std::unordered_set<const Text*, Text::Hash, Text::Equals> TextSet;

  const TextSet& GetAll();
}

#endif //MCC_TEXT_H