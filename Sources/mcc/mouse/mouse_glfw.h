#ifndef MCC_MOUSE_H
#error "Please #include <mcc/mouse/mouse.h> instead."
#endif //MCC_MOUSE_H

#ifndef MCC_MOUSE_GLFW_H
#define MCC_MOUSE_GLFW_H

#include <optional>

#include "mcc/mouse/mouse.h"

namespace mcc {
  class Window;
}

namespace mcc::mouse {
#define FOR_EACH_MOUSE_INPUT_MODE(V)             \
  V(Disabled,     GLFW_CURSOR_DISABLED)          \
  V(Hidden,       GLFW_CURSOR_HIDDEN)            \
  V(Normal,       GLFW_CURSOR_NORMAL)

  class GlfwMouse : public Mouse {
    friend class Engine;
  public:
    enum InputMode : int {
#define DEFINE_INPUT_MODE(Name, GlfwValue) k##Name = (GlfwValue),
      FOR_EACH_MOUSE_INPUT_MODE(DEFINE_INPUT_MODE)
#undef DEFINE_INPUT_MODE
    };

    friend std::ostream& operator<<(std::ostream& stream, const InputMode& rhs) {
      switch(rhs) {
#define DEFINE_TO_STRING(Name, GlfwValue)                                     \
        case k##Name: return stream << #Name << " (" << #GlfwValue << ")";
        FOR_EACH_MOUSE_INPUT_MODE(DEFINE_TO_STRING)
        default: return stream << "Unknown Mouse::InputMode";
      }
#undef DEFINE_TO_STRING
    }

    static constexpr const auto kDefaultInputMode = kNormal;

    static inline std::optional<InputMode>
    ParseInputMode(const std::string& value) {
      if(EqualsIgnoreCase(value, "default")) 
        return kDefaultInputMode;
#define DEFINE_PARSE_INPUT_MODE(Name, GlfwValue)        \
      else if(EqualsIgnoreCase(value, #Name))           \
        return { k##Name };
      FOR_EACH_MOUSE_INPUT_MODE(DEFINE_PARSE_INPUT_MODE)
#undef DEFINE_PARSE_INPUT_MODE
      DLOG(WARNING) << "failed to parse GlfwMouse::InputMode from: " << value;
      return std::nullopt;
    }

    InputMode GetInputMode() const;
  protected:
    Window* window_;
    MouseButtonSet buttons_;
    MouseButtonStateSet states_;
    glm::vec2 pos_;
    glm::vec2 last_pos_;
    glm::vec2 delta_;

    void OnPreTick(engine::PreTickEvent* event) override;

    inline Window* GetWindow() const {
      return window_;
    }

    GLFWwindow* GetWindowHandle() const;
  public:
    explicit GlfwMouse(engine::Engine* engine, Window* window);
    ~GlfwMouse() override = default;


    bool IsCursorEnabled() const override {
      switch(GetInputMode()) {
        case kNormal:
        case kHidden:
          return true;
        case kDisabled:
        default:
          return false;
      }
    }

    bool IsCursorDisabled() const override {
      switch(GetInputMode()) {
        case kDisabled:
          return true;
        case kNormal:
        case kHidden:
        default:
          return false;
      }
    }

    bool IsCursorHidden() const override {
      switch(GetInputMode()) {
        case kHidden:
          return true;
        case kNormal:
        case kDisabled:
        default:
          return true;
      }
    }

    bool IsCursorVisible() const override {
      switch(GetInputMode()) {
        case kNormal:
          return true;
        case kHidden:
        case kDisabled:
        default:
          return false;
      }
    }

    inline Window* window() const {
      return window_;
    }

    void SetCursorPos(const glm::vec2& pos) override;

    glm::vec2 GetPosition() const override {
      return pos_;
    }

    glm::vec2 GetDelta() const override {
      return delta_;
    }

    glm::vec2 GetCursorPosition() const override;
    glm::vec2 GetNormalizedPosition() const override;

    rx::observable<MouseButton> GetAllButtons() const override {
      return rx::observable<>::iterate(buttons_);
    }

    rx::observable<std::pair<MouseButton, MouseButton::State>> GetAllButtonStates() const override {
      return GetAllButtons()
        .map([this](const MouseButton& btn) {
          return std::make_pair(btn, btn.GetState(GetWindowHandle()));
        });
    }
  public:
    static inline GlfwMouse* New();
  };
}

#endif //MCC_MOUSE_GLFW_H