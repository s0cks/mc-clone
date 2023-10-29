configure_file(mcc/mcc.h.in ${PROJECT_BINARY_DIR}/mcc/mcc.h)
include_directories(${PROJECT_BINARY_DIR})
include_directories(${LIBUV_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})

add_library(${PROJECT_NAME}
  ${PROJECT_BINARY_DIR}/mcc/mcc.h mcc/mcc.cc
  mcc/window.h mcc/window.cc
  mcc/shader/shader.h mcc/shader/shader.cc
  mcc/timestamp.h mcc/timestamp.cc
  mcc/engine/engine_state.h
  mcc/engine/engine.h mcc/engine/engine.cc
  mcc/engine/engine_stage.h mcc/engine/engine_stage.cc
  mcc/engine/engine_stage_preinit.h
  mcc/engine/engine_stage_init.h
  mcc/engine/engine_stage_postinit.h
  mcc/engine/engine_stage_tick.h
  mcc/engine/engine_stage_terminating.h
  mcc/engine/engine_stage_terminated.h
  mcc/gfx.h
  mcc/common.h
  mcc/flags.h mcc/flags.cc
  mcc/platform.h
  mcc/vao.h mcc/vao.cc
  mcc/vbo.h mcc/vbo.cc
  mcc/ibo.h mcc/ibo.cc
  mcc/mesh/vertex.h mcc/mesh/vertex.cc
  mcc/mesh/index.h
  mcc/shape/plane.h mcc/shape/plane.cc
  mcc/event/event_bus.h mcc/event/event_bus.cc
  mcc/mesh/mesh.h mcc/mesh/mesh.cc
  mcc/render_loop.h mcc/render_loop.cc
  mcc/buffer.h mcc/buffer.cc
  mcc/uv_utils.h
  mcc/os_thread.h mcc/os_thread.cc
  mcc/os_thread_linux.h
  mcc/os_thread_osx.h mcc/os_thread_osx.cc
  mcc/thread_local.h
  mcc/relaxed_atomic.h
  mcc/fixed_rate_loop.h mcc/fixed_rate_loop.cc
  mcc/trie.h
  mcc/subscription_trie.h
  mcc/mouse/mouse_constants.h
  mcc/mouse/mouse_trie.h
  mcc/mouse/mouse.h mcc/mouse/mouse.cc
  mcc/keyboard/keyboard_constants.h
  mcc/keyboard/keyboard.h mcc/keyboard/keyboard.cc
  mcc/keyboard/keyboard_trie.h mcc/keyboard/keyboard_trie.cc
  mcc/shape/shape.h
  mcc/shape/square.h
  mcc/shape/cube.h mcc/shape/cube.cc
  mcc/texture/texture.h mcc/texture/texture.cc
  mcc/terrain/terrain.h mcc/terrain/terrain.cc
  mcc/murmur.h
  mcc/bitfield.h
  mcc/font/font.h mcc/font/font.cc
  mcc/font/glyph.h mcc/font/glyph.cc
  mcc/font/font_mesh.h mcc/font/font_mesh.cc
  mcc/font/truetype.h
  mcc/font/font_renderer.h mcc/font/font_renderer.cc
  mcc/scene.h mcc/scene.cc
  mcc/mesh/model.h mcc/mesh/model.cc
  mcc/rotation.h mcc/rotation.cc
  mcc/renderer/renderable.h mcc/renderer/renderable.cc
  mcc/renderer/renderer.h mcc/renderer/renderer.cc
  mcc/camera/camera.h mcc/camera/camera.cc
  mcc/camera/perspective_camera.h mcc/camera/perspective_camera.cc
  mcc/ecs/ecs.h
  mcc/ecs/coordinator.h
  mcc/ecs/entity.h mcc/ecs/entity.cc
  mcc/ecs/component.h mcc/ecs/component.cc
  mcc/ecs/system.h mcc/ecs/system.cc
  mcc/ecs/event.h mcc/ecs/event.cc)
target_link_libraries(${PROJECT_NAME}
  Threads::Threads
  glog::glog
  ${GFLAGS_LIBRARIES}
  OpenGL::GL
  glfw
  GLEW::GLEW
  PNG::PNG
  Freetype::Freetype
  ${LIBUV_LIBRARIES})

if(CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wno-deprecated)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/Sources ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME}-client
  main.cc)
target_link_libraries(${PROJECT_NAME}-client
  ${PROJECT_NAME})