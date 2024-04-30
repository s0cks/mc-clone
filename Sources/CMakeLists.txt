configure_file(mcc/mcc.h.in ${PROJECT_BINARY_DIR}/mcc/mcc.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})
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

file(GLOB MCC_FBO_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/fbo/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/fbo/*.cc")

file(GLOB MCC_RBO_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/rbo/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/rbo/*.cc")

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

add_library(${PROJECT_NAME}
  ${PROJECT_BINARY_DIR}/mcc/mcc.h mcc/mcc.cc
  ${MCC_COMPONENT_SOURCE_FILES}
  ${MCC_KEYBOARD_SOURCE_FILES}
  ${MCC_MOUSE_SOURCE_FILES}
  ${MCC_CAMERA_SOURCE_FILES}
  ${MCC_ENGINE_SOURCE_FILES}
  ${MCC_VAO_SOURCE_FILES}
  ${MCC_VBO_SOURCE_FILES}
  ${MCC_IBO_SOURCE_FILES}
  ${MCC_PROGRAM_SOURCE_FILES}
  ${MCC_RENDER_SOURCE_FILES}
  ${MCC_SHADER_SOURCE_FILES}
  ${MCC_WINDOW_SOURCE_FILES}
  ${MCC_IMAGE_SOURCE_FILES}
  ${MCC_TEXTURE_SOURCE_FILES}
  ${MCC_RBO_SOURCE_FILES}
  ${MCC_FBO_SOURCE_FILES}
  ${MCC_ENTITY_SOURCE_FILES}
  ${MCC_GUI_SOURCE_FILES}
  ${MCC_RESOURCE_SOURCE_FILES}
  ${MCC_LIBUV_SOURCE_FILES}
  ${MCC_FONT_SOURCE_FILES}
  ${MCC_UBO_SOURCE_FILES}
  ${MCC_SETTINGS_SOURCE_FILES}
# Misc
  mcc/murmur.h
  mcc/bitfield.h
  mcc/uv.h
  mcc/on_shutdown.h
  mcc/event.h
  mcc/state_machine.h
  mcc/bitset.h
  mcc/uri.h mcc/uri.cc
  mcc/json.h mcc/json.cc
  mcc/atomic_linked_list.h
  mcc/wsq.h
  mcc/predicate.h
  mcc/file_resolver.h
  mcc/common.h
  mcc/platform.h
  mcc/buffer.h mcc/buffer.cc
  mcc/cache.h mcc/cache.cc
  mcc/timestamp.h mcc/timestamp.cc
  mcc/pipeline.h mcc/pipeline.cc
  mcc/parser.h
  mcc/resolution.h mcc/resolution.cc
  mcc/cull_face_scope.h mcc/cull_face_scope.cc
  mcc/reference.h
  mcc/aabb.h mcc/aabb.cc
  mcc/transform.h mcc/transform.cc
  mcc/gfx.h
  mcc/buffer_object.h
  mcc/uniform_buffer.h
  mcc/renderbuffer.h
  mcc/rx.h
  mcc/color.h mcc/color.cc
  mcc/shape.h
  mcc/glm.h
  mcc/flags.h mcc/flags.cc
  mcc/os_thread.h mcc/os_thread.cc
  mcc/os_thread_linux.h
  mcc/os_thread_osx.h mcc/os_thread_osx.cc
  mcc/thread_local.h
  mcc/relaxed_atomic.h
  mcc/trie.h
  mcc/circular_buffer.h
  mcc/series.h
  mcc/subscription_trie.h)
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