// #ifndef MCC_WINDOW_H
// #define MCC_WINDOW_H

// #include "mcc/rx.h"
// #include "mcc/gfx.h"
// #include "mcc/flags.h"
// #include "mcc/common.h"
// #include "mcc/gui/gui_frame.h"

// namespace mcc {
//   struct WindowResizedEvent {
//     GLFWwindow* window;
//     Dimension size;
//   };

//   class Window {
//     DEFINE_NON_INSTANTIABLE_TYPE(Window);
//   private:
//     static void SetHandle(GLFWwindow* handle);
//     static void SetSize(const glm::vec2 size);
//     static void OnPreInit();
//     static void OnInit();
//     static void OnPostInit();
//     static void OnTerminating();
//     static void OnTerminated();
//     static void OnWindowClosed(GLFWwindow* window);
//   public:
//     static void Init();
//     static void Close();
//     static GLFWwindow* GetHandle();
//     static glm::vec2 GetSize();
//     static rx::observable<WindowResizedEvent*> OnResized();

//     static float GetWidth();
//     static float GetHeight();

//     static void AddFrame(gui::FramePtr frame);
//     static bool VisitFrames(gui::FrameVisitor* vis);

//     static inline float
//     GetAspectRatio() {
//       return GetWidth() / GetHeight();
//     }

//     static inline glm::vec2
//     GetCenterCoord() {
//       return glm::vec2(GetWidth() / 2, GetHeight() / 2);
//     }

//     static inline bool
//     IsFullscreen() {
//       return FLAGS_fullscreen;
//     }

//     static inline glm::vec2
//     GetInitialSize() {
//       return glm::vec2(static_cast<float>(FLAGS_width), static_cast<float>(FLAGS_height));
//     }
//   };
// }

// #endif //MCC_WINDOW_H