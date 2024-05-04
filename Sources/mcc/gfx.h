#ifndef MCC_GFX_H
#define MCC_GFX_H

#include "mcc/common.h"
#ifdef OS_IS_OSX

#define GL_SILENCE_DEPRECATION 1
#include <OpenGL/gl3.h>

#else

#include <GL/gl3.h>

#endif // OS_IS_OSX

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <units.h>
#include <iostream>
#include <glog/logging.h>

#include "mcc/glm.h"
#include "mcc/gfx_error.h"
#include "mcc/gfx_misc.h"

#ifdef M_PI
#define PI M_PI
#endif //M_PI

#ifndef PI
#define PI 3.141592654
#endif //PI

#endif //MCC_GFX_H