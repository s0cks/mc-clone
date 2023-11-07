#ifndef MCC_MOCK_GLFW_H
#define MCC_MOCK_GLFW_H

#include <gmock/gmock.h>
#include "mcc/gfx.h"

namespace mcc {
  class MockGlfw {
  public:
    MockGlfw();
    ~MockGlfw();
    MOCK_METHOD1(glfwSwapInterval, void(int interval));
    MOCK_METHOD5(glfwCreateWindow, GLFWwindow*(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share));
    MOCK_METHOD3(glfwGetCursorPos, void(GLFWwindow* window, double* xpos, double* ypos));
    MOCK_METHOD3(glfwGetFramebufferSize, void(GLFWwindow* window, int* width, int* height));
    MOCK_METHOD0(glfwTerminate, void(void));
    MOCK_METHOD2(glfwGetKey, int(GLFWwindow* window, int key));
    MOCK_METHOD1(glfwSwapBuffers, void(GLFWwindow* window));
    MOCK_METHOD0(glfwPollEvents, void(void));
    MOCK_METHOD2(glfwSetWindowShouldClose, void(GLFWwindow* window, int value));
    MOCK_METHOD2(glfwGetMouseButton, int(GLFWwindow* window, int button));
    MOCK_METHOD2(glfwSetWindowCloseCallback, GLFWwindowclosefun(GLFWwindow* window, GLFWwindowclosefun callback));
    MOCK_METHOD1(glfwMakeContextCurrent, void(GLFWwindow* window));
    MOCK_METHOD3(glfwGetWindowSize, void(GLFWwindow* window, int* width, int* height));
  };
}

#endif //MCC_MOCK_GLFW_H