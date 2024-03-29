// #include <glog/logging.h>
// #include <uv.h>
// #include <cstdlib>
// #include <cstdio>

// #include "mcc/gui/gui.h"

// #include "mcc/flags.h"
// #include "mcc/window.h"
// #include "mcc/shader/shader.h"
// #include "mcc/camera/camera.h"
// #include "mcc/thread_local.h"
// #include "mcc/uv_utils.h"

// #include "mcc/font/font.h"
// #include "mcc/font/font_renderer.h"
// #include "mcc/mesh/mesh.h"

// #include "mcc/input/mouse.h"
// #include "mcc/input/keyboard.h"

// #include "mcc/engine/engine.h"
// #include "mcc/renderer/renderer.h"
// #include "mcc/camera/perspective_camera.h"

// #include "mcc/thread_local.h"
// #include "mcc/gui/gui_frame.h"

// #include "mcc/physics/force.h"
// #include "mcc/physics/transform.h"
// #include "mcc/physics/rigid_body.h"

// #include "mcc/light/point.h"
// #include "mcc/light/directional.h"

// #include "mcc/shader/shader.h"

// #include "mcc/resource.h"

// namespace mcc {
//   static ThreadLocal<GLFWwindow> handle_;
//   static rxsub::subject<WindowResizedEvent*> on_resized_;

//   static glm::vec2 size_;
//   static std::string title_;
//   static std::vector<gui::FramePtr> frames_;

//   void Window::AddFrame(gui::FramePtr frame) {
//     frames_.push_back(frame);
//   }

//   bool Window::VisitFrames(gui::FrameVisitor* vis) {
//     for(auto& frame : frames_) {
//       if(!vis->Visit(frame))
//         return false;
//     }
//     return true;
//   }

//   void Window::SetHandle(GLFWwindow* handle) {
//     handle_.Set(handle);
//   }

//   GLFWwindow* Window::GetHandle() {
//     return handle_.Get();
//   }

//   void Window::SetSize(const glm::vec2 size) {
//     size_ = size;
//   }

//   glm::vec2 Window::GetSize() {
//     return size_;
//   }

//   float Window::GetWidth() {
//     return size_[0];
//   }

//   float Window::GetHeight() {
//     return size_[1];
//   }

//   static inline void
//   OnWindowResized(GLFWwindow* window, int width, int height) {
//     const auto orthoCamera = OrthoCamera::Get();
//     orthoCamera->SetSize(glm::vec2(width, height));
//     const auto perspectiveCamera = PerspectiveCamera::Get();
//     perspectiveCamera->SetSize(glm::vec2(width, height));
//   }

//   void Window::Close() {
//     glfwSetWindowShouldClose(GetHandle(), GL_TRUE);
//   }

//   void Window::OnWindowClosed(GLFWwindow* window) {
//     Engine::Shutdown();
//   }

//   rx::observable<WindowResizedEvent*> Window::OnResized() {
//     return on_resized_.get_observable();
//   }
// }