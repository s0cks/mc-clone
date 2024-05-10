#ifndef MCC_SHADER_SET_H
#define MCC_SHADER_SET_H

#include "mcc/rx.h"
#include "mcc/shader/shader_id.h"
#include "mcc/shader/shader_type.h"

namespace mcc {
  namespace shader {
    class Shader;
    class ShaderSet {
    public:
      class Item {
        DEFINE_DEFAULT_COPYABLE_TYPE(Item);
        friend class ShaderSet;
      protected:
        ShaderId id_;
        shader::ShaderType type_;

        explicit Item(const Shader* shader);
      public:
        ~Item() = default;
        
        ShaderId id() const {
          return id_;
        }

        shader::ShaderType type() const {
          return type_;
        }

        friend std::ostream& operator<<(std::ostream& stream, const Item& rhs) {
          stream << "ShaderSet::Item(";
          stream << "id=" << rhs.id() << ", ";
          stream << "type=" << rhs.type();
          stream << ")";
          return stream;
        }

        bool operator==(const Item& rhs) const {
          return type() == rhs.type();
        }

        bool operator!=(const Item& rhs) const {
          return type() != rhs.type();
        }

        bool operator<(const Item& rhs) const {
          return type() < rhs.type();
        }

        bool operator>(const Item& rhs) const {
          return type() > rhs.type();
        }
      };
      typedef std::set<Item> ItemSet;
    protected:
      ItemSet items_;
    public:
      ShaderSet() = default;
      virtual ~ShaderSet() = default;

      ItemSet::const_iterator begin() const {
        return std::begin(items_);
      }

      ItemSet::const_iterator end() const {
        return std::end(items_);
      }
      
      virtual bool Contains(const Shader* shader) const {
        if(!shader)
          return false;
        const auto item = Item(shader);
        const auto pos = items_.find(item);
        return pos != items_.end();
      }

      virtual bool Insert(const Shader* shader) {
        if(!shader)
          return false;
        const auto item = Item(shader);
        const auto result = items_.insert(item);
        return result.second;
      }

      virtual rx::observable<ShaderId> ToObservable() const {
        return rx::observable<>::iterate(items_)
          .map([](const Item& item) {
            return item.id();
          });
      }
    };
  }
  using shader::ShaderSet;
}

#endif //MCC_SHADER_SET_H