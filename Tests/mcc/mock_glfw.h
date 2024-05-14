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
    MOCK_METHOD3(glfwSetInputMode, void(GLFWwindow* window, int mode, int value));
    MOCK_METHOD0(glfwGetPrimaryMonitor, GLFWmonitor*());
    MOCK_METHOD3(glfwGetWindowPos, void(GLFWwindow* window, int* xPos, int* yPos));
    MOCK_METHOD1(glfwGetWindowUserPointer, void*(GLFWwindow*));
    MOCK_METHOD2(glfwSetWindowUserPointer, void(GLFWwindow*, void* data));
    MOCK_METHOD2(glfwSetWindowContentScaleCallback, GLFWwindowcontentscalefun(GLFWwindow*, GLFWwindowcontentscalefun callback));
    MOCK_METHOD2(glfwSetWindowFocusCallback, GLFWwindowfocusfun(GLFWwindow*, GLFWwindowfocusfun));
    MOCK_METHOD2(glfwSetWindowIconifyCallback, GLFWwindowiconifyfun(GLFWwindow*, GLFWwindowiconifyfun));
    MOCK_METHOD2(glfwSetWindowMaximizeCallback, GLFWwindowmaximizefun(GLFWwindow*, GLFWwindowmaximizefun));
    MOCK_METHOD3(glfwSetWindowPos, void(GLFWwindow*,int,int));
    MOCK_METHOD3(glfwSetWindowSize, void(GLFWwindow*,int,int));
    MOCK_METHOD2(glfwSetWindowTitle, void(GLFWwindow*, const char*));
    MOCK_METHOD2(glfwSetWindowPosCallback, GLFWwindowposfun(GLFWwindow*, GLFWwindowposfun));
    MOCK_METHOD2(glfwSetWindowRefreshCallback, GLFWwindowrefreshfun(GLFWwindow*, GLFWwindowrefreshfun));
    MOCK_METHOD2(glfwSetWindowSizeCallback, GLFWwindowsizefun(GLFWwindow*,GLFWwindowsizefun));
    MOCK_METHOD1(glfwGetVideoMode, const GLFWvidmode*(GLFWmonitor*));
    MOCK_METHOD1(glfwCreateStandardCursor, GLFWcursor*(const int shape));
  };
}

#endif //MCC_MOCK_GLFW_H