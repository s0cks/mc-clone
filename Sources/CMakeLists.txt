configure_file(mcc/mcc.h.in ${PROJECT_BINARY_DIR}/Sources/mcc/mcc.h)
configure_file(mcc/gfx.h.in ${PROJECT_BINARY_DIR}/Sources/mcc/gfx.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${LIBUV_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${UTF8_INCLUDE_DIRS})

file(GLOB MCC_COMPONENT_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/component/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/component/*.cc")

file(GLOB MCC_KEYBOARD_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/keyboard/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/keyboard/*.cc")

file(GLOB MCC_MOUSE_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/mouse/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/mouse/*.cc")

file(GLOB MCC_CAMERA_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/camera/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/camera/*.cc")

file(GLOB MCC_PROGRAM_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/program/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/program/*.cc")

file(GLOB MCC_SHADER_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/shader/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/shader/*.cc")

file(GLOB MCC_RENDER_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/render/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/render/*.cc")

file(GLOB MCC_GUI_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/gui/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/gui/*.cc")

file(GLOB MCC_VAO_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/vao/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/vao/*.cc")

file(GLOB MCC_VBO_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/vbo/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/vbo/*.cc")

file(GLOB MCC_IBO_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/ibo/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/ibo/*.cc")

file(GLOB MCC_UBO_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/ubo/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/ubo/*.cc")

file(GLOB MCC_FBO_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/fbo/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/fbo/*.cc")

file(GLOB MCC_RBO_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/rbo/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/rbo/*.cc")

file(GLOB MCC_ENGINE_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/engine/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/engine/*.cc")

file(GLOB MCC_TEXTURE_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/texture/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/texture/*.cc")

file(GLOB MCC_IMAGE_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/image/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/image/*.cc")

file(GLOB MCC_WINDOW_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/window/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/window/*.cc")

file(GLOB MCC_RESOURCE_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/resource/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/resource/*.cc")

file(GLOB MCC_LIBUV_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/uv/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/uv/*.cc")

file(GLOB MCC_ENTITY_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/entity/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/entity/*.cc")

file(GLOB MCC_FONT_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/font/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/font/*.cc")

file(GLOB MCC_MATERIAL_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/material/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/material/*.cc")

file(GLOB MCC_SETTINGS_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/settings/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/settings/*.cc")

file(GLOB MCC_MISC_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/*.cc")

add_library(${PROJECT_NAME}
  ${PROJECT_BINARY_DIR}/Sources/mcc/mcc.h mcc/mcc.cc
  ${PROJECT_BINARY_DIR}/Sources/mcc/gfx.h
  ${MCC_COMPONENT_SOURCE_FILES}
  ${MCC_KEYBOARD_SOURCE_FILES}
  ${MCC_MOUSE_SOURCE_FILES}
  ${MCC_CAMERA_SOURCE_FILES}
  ${MCC_ENGINE_SOURCE_FILES}
  ${MCC_VAO_SOURCE_FILES}
  ${MCC_VBO_SOURCE_FILES}
  ${MCC_IBO_SOURCE_FILES}
  ${MCC_RBO_SOURCE_FILES}
  ${MCC_FBO_SOURCE_FILES}
  ${MCC_UBO_SOURCE_FILES}
  ${MCC_PROGRAM_SOURCE_FILES}
  ${MCC_RENDER_SOURCE_FILES}
  ${MCC_SHADER_SOURCE_FILES}
  ${MCC_WINDOW_SOURCE_FILES}
  ${MCC_IMAGE_SOURCE_FILES}
  ${MCC_TEXTURE_SOURCE_FILES}
  ${MCC_ENTITY_SOURCE_FILES}
  ${MCC_GUI_SOURCE_FILES}
  ${MCC_RESOURCE_SOURCE_FILES}
  ${MCC_LIBUV_SOURCE_FILES}
  ${MCC_FONT_SOURCE_FILES}
  ${MCC_SETTINGS_SOURCE_FILES}
  ${MCC_MATERIAL_SOURCE_FILES}
  ${MCC_MISC_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
  PNG::PNG
  JPEG::JPEG
  ${LEVELDB_LIBRARIES}
  flatbuffers::flatbuffers
  Freetype::Freetype
  units::units
  semver
  fmt
  vorbis
  tweeny
  OpenAL::OpenAL
  Backward::Backward
  OpenSSL::Crypto
  ${LIBUV_LIBRARIES})

if(CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wno-deprecated -frtti)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/Sources ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME}-client
  main.cc
  )
target_link_libraries(${PROJECT_NAME}-client
  ${PROJECT_NAME}
  OpenGL::GL
  glfw
  GLEW::GLEW)