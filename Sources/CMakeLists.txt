configure_file(mcc/mcc.h.in ${PROJECT_BINARY_DIR}/mcc/mcc.h)
include_directories(${PROJECT_BINARY_DIR})
include_directories(${LIBUV_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})

add_library(${PROJECT_NAME}
  ${PROJECT_BINARY_DIR}/mcc/mcc.h mcc/mcc.cc
  mcc/window.h mcc/window.cc
  mcc/shader/shader.h mcc/shader/shader.cc
  mcc/shader/compiler_result.h
  mcc/shader/compiler.h mcc/shader/compiler.cc
  mcc/shader/builder.h mcc/shader/builder.cc
  mcc/shader/source.h
  mcc/cache.h mcc/cache.cc
  mcc/timestamp.h mcc/timestamp.cc
  mcc/engine/engine_stats.h
  mcc/engine/engine_state.h
  mcc/engine/engine.h mcc/engine/engine.cc
  mcc/engine/engine_stage.h mcc/engine/engine_stage.cc
  mcc/engine/engine_stage_preinit.h
  mcc/engine/engine_stage_init.h
  mcc/engine/engine_stage_postinit.h
  mcc/engine/engine_stage_tick.h
  mcc/engine/engine_stage_terminating.h
  mcc/engine/engine_stage_terminated.h
  mcc/component/component_id.h
  mcc/component/component_state.h
  mcc/component/component.h mcc/component/component.cc
  mcc/aabb.h mcc/aabb.cc
  mcc/physics/rigid_body.h mcc/physics/rigid_body.cc
  mcc/physics/force.h mcc/physics/force.cc
  mcc/physics/transform.h mcc/physics/transform.cc
  mcc/physics/collision.h mcc/physics/collision.cc
  mcc/physics/physics.h mcc/physics/physics.cc
  mcc/gfx.h
  mcc/rx.h
  mcc/common.h
  mcc/flags.h mcc/flags.cc
  mcc/platform.h
  mcc/mesh/vertex.h mcc/mesh/vertex.cc
  mcc/mesh/index.h
  mcc/mesh/mesh.h mcc/mesh/mesh.cc
  mcc/buffer.h mcc/buffer.cc
  mcc/uv_utils.h
  mcc/os_thread.h mcc/os_thread.cc
  mcc/os_thread_linux.h
  mcc/os_thread_osx.h mcc/os_thread_osx.cc
  mcc/thread_local.h
  mcc/relaxed_atomic.h
  mcc/trie.h
  mcc/circular_buffer.h
  mcc/series.h
  mcc/framebuffer.h mcc/framebuffer.cc
  mcc/subscription_trie.h
  mcc/input/constants.h
  mcc/input/mouse.h mcc/input/mouse.cc
  mcc/input/keyboard.h mcc/input/keyboard.cc
  mcc/texture/texture.h mcc/texture/texture.cc
  mcc/texture/texture_cache_flags.h mcc/texture/texture_cache.cc
  mcc/texture/texture_cache.h mcc/texture/texture_cache.cc
  mcc/texture/texture_loader.h mcc/texture/texture_loader.cc
  mcc/terrain/terrain_flags.h
  mcc/terrain/terrain.h mcc/terrain/terrain.cc
  mcc/terrain/terrain_renderer.h mcc/terrain/terrain_renderer.cc
  mcc/terrain/terrain_vertex.h
  mcc/terrain/terrain_chunk.h mcc/terrain/terrain_chunk.cc
  mcc/murmur.h
  mcc/settings.h mcc/settings.cc
  mcc/bitfield.h
  mcc/lighting/ambient_light.h mcc/lighting/ambient_light.cc
  mcc/font/font.h mcc/font/font.cc
  mcc/font/glyph.h mcc/font/glyph.cc
  mcc/font/font_mesh.h mcc/font/font_mesh.cc
  mcc/font/truetype.h
  mcc/font/font_renderer.h mcc/font/font_renderer.cc
  mcc/camera/camera.h mcc/camera/camera.cc
  mcc/camera/perspective_camera.h mcc/camera/perspective_camera.cc
  mcc/ecs/coordinator.h
  mcc/ecs/entity.h mcc/ecs/entity.cc
  mcc/gui/gui.h mcc/gui/gui.cc
  mcc/gui/gui_frame.h mcc/gui/gui_frame.cc
  mcc/gui/gui_frame_settings.h mcc/gui/gui_frame_settings.cc
  mcc/gui/gui_frame_renderer.h mcc/gui/gui_frame_renderer.cc
  mcc/state_machine.h
  mcc/renderer/renderer_state.h
  mcc/renderer/renderer_stats.h
  mcc/renderer/stage.h mcc/renderer/stage.cc
  mcc/renderer/stage_render_pre.cc
  mcc/renderer/stage_render_post.cc
  mcc/renderer/stage_render_terrain.cc
  mcc/renderer/stage_render_entities.cc
  mcc/renderer/stage_render_gui.cc
  mcc/renderer/stage_render_screen.cc
  mcc/renderer/renderable.h mcc/renderer/renderable.cc
  mcc/renderer/renderer.h mcc/renderer/renderer.cc)
target_link_libraries(${PROJECT_NAME}
  Threads::Threads
  glog::glog
  ${GFLAGS_LIBRARIES}
  PNG::PNG
  ${LEVELDB_LIBRARIES}
  Freetype::Freetype
  ${LIBUV_LIBRARIES})

if(CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wno-deprecated)
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