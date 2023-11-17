#include "mcc/mock_glfw.h"
#include "mcc/thread_local.h"

namespace mcc {
  static ThreadLocal<MockGlfw> mock_;

  MockGlfw::MockGlfw() {
    mock_.Set(this);
  }

  MockGlfw::~MockGlfw() {
    mock_.Set(nullptr);
  }
}

static inline mcc::MockGlfw*
GetMock() {
  const auto mock = mcc::mock_.Get();
  assert(mock != nullptr);
  return mock;
}

#define __ return GetMock()->

void glfwSwapInterval(int interval) {
  __ glfwSwapInterval(interval);
}

GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
  __ glfwCreateWindow(width, height, title, monitor, share);
}

void glfwGetCursorPos(GLFWwindow* window, double* xpos, double* ypos) {
  __ glfwGetCursorPos(window, xpos, ypos);
}

void glfwGetFramebufferSize(GLFWwindow* window, int* width, int* height) {
  __ glfwGetFramebufferSize(window, width, height);
}

void glfwTerminate(void) {
  __ glfwTerminate();
}

int glfwGetKey(GLFWwindow* window, int key) {
  __ glfwGetKey(window, key);
}

void glfwSwapBuffers(GLFWwindow* window) {
  __ glfwSwapBuffers(window);
}

void glfwPollEvents(void) {
  __ glfwPollEvents();
}

void glfwSetWindowShouldClose(GLFWwindow* window, int value) {
  __ glfwSetWindowShouldClose(window, value);
}

int glfwGetMouseButton(GLFWwindow* window, int button) {
  __ glfwGetMouseButton(window, button);
}

GLFWwindowclosefun glfwSetWindowCloseCallback(GLFWwindow* window, GLFWwindowclosefun callback) {
  __ glfwSetWindowCloseCallback(window, callback);
}

void glfwMakeContextCurrent(GLFWwindow* window) {
  __ glfwMakeContextCurrent(window);
}

void glfwGetWindowSize(GLFWwindow* window, int* width, int* height) {
  __ glfwGetWindowSize(window, width, height);
}

void glfwSetInputMode(GLFWwindow* window, int mode, int value) {
  __ glfwSetInputMode(window, mode, value);
}