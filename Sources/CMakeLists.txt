configure_file(mcc/mcc.h.in ${PROJECT_BINARY_DIR}/mcc/mcc.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${LIBUV_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${UTF8_INCLUDE_DIRS})

# set(FB_SCHEMAS
#   "${CMAKE_CURRENT_SOURCE_DIR}/mcc/materials.fbs")
# build_flatbuffers("${FB_SCHEMAS}"
#                   "${CMAKE_CURRENT_SOURCE_DIR}/mcc/"
#                   fbschemas
#                   ""
#                   "${PROJECT_BINARY_DIR}/mcc"
#                   ""
#                   "")

add_library(${PROJECT_NAME}
  ${PROJECT_BINARY_DIR}/mcc/mcc.h mcc/mcc.cc
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
  mcc/reference.h
  mcc/component/component_id.h
  mcc/component/component_table.h mcc/component/component_table.cc
  mcc/component/component_state.h
  mcc/component/component.h mcc/component/component.cc
  mcc/aabb.h mcc/aabb.cc
  mcc/physics/rigid_body.h mcc/physics/rigid_body.cc
  mcc/physics/force.h mcc/physics/force.cc
  mcc/physics/transform.h mcc/physics/transform.cc
  mcc/physics/collision.h mcc/physics/collision.cc
  mcc/physics/physics.h mcc/physics/physics.cc
  mcc/material/material.h mcc/material/material.cc
  mcc/material/material_json.h mcc/material/material_json.cc
  mcc/material/material_loader.h mcc/material/material_loader.cc
  mcc/gfx.h
  mcc/vao.h
  mcc/buffer_object.h
  mcc/index_buffer.h
  mcc/uniform_buffer.h
  mcc/renderbuffer.h
  mcc/bloom.h mcc/bloom.cc
  mcc/rx.h
  mcc/glm.h
  mcc/flags.h mcc/flags.cc
  mcc/mesh/vertex.h mcc/mesh/vertex.cc
  mcc/mesh/mesh.h mcc/mesh/mesh.cc
  mcc/mesh/mesh_loader.h mcc/mesh/mesh_loader.cc
  mcc/mesh/obj_parser.h mcc/mesh/obj_parser.cc
  mcc/uv_utils.h
  mcc/os_thread.h mcc/os_thread.cc
  mcc/os_thread_linux.h
  mcc/os_thread_osx.h mcc/os_thread_osx.cc
  mcc/thread_local.h
  mcc/relaxed_atomic.h
  mcc/trie.h
  mcc/circular_buffer.h
  mcc/series.h
  mcc/framebuffer_constants.h
  mcc/framebuffer_object.h
  mcc/framebuffer_attachment.h
  mcc/framebuffer.h mcc/framebuffer.cc
  mcc/framebuffer_pipeline.h mcc/framebuffer_pipeline.cc
  mcc/subscription_trie.h
  mcc/skybox.h mcc/skybox.cc
# Task
# Keyboard
  mcc/keyboard/keyboard_constants.h
  mcc/keyboard/key_state_set.h
  mcc/keyboard/keyboard_event.h mcc/keyboard/keyboard_event.cc
  mcc/keyboard/keyboard.h mcc/keyboard/keyboard.cc
  mcc/keyboard/keyboard_glfw.h mcc/keyboard/keyboard_glfw.cc
# Mouse
  mcc/mouse/mouse_constants.h
  mcc/mouse/mouse_event.h mcc/mouse/mouse_event.cc
  mcc/mouse/mouse.h mcc/mouse/mouse.cc
  mcc/mouse/mouse_glfw.h mcc/mouse/mouse_glfw.cc
# Camera
  mcc/camera/camera_ortho.h mcc/camera/camera_ortho.cc
# Render
  mcc/renderer/projection_matrix.h mcc/renderer/projection_matrix.cc
  mcc/renderer/render_pass.h mcc/renderer/render_pass.cc
  mcc/renderer/render_pass_2d.h mcc/renderer/render_pass_2d.cc
  mcc/renderer/render_pass_3d.h mcc/renderer/render_pass_3d.cc
  mcc/renderer/render_pass_executor.h mcc/renderer/render_pass_executor.cc
  mcc/renderer/renderer_state.h
  mcc/renderer/renderer_stats.h
  mcc/renderer/renderable.h mcc/renderer/renderable.cc
  mcc/renderer/renderer.h mcc/renderer/renderer.cc
# Vertex
  mcc/vertex/vertex_property.h
  mcc/vertex/vertex.h mcc/vertex/vertex.cc
  mcc/vertex/vertex_buffer.h
  mcc/vertex/vertex_2d.h mcc/vertex/vertex_2d.cc
  mcc/vertex/vertex_3d.h mcc/vertex/vertex_3d.cc
# Engine
  mcc/engine/engine_event.h mcc/engine/engine_event.cc
  mcc/engine/engine.h mcc/engine/engine.cc
  mcc/engine/engine_stage.h mcc/engine/engine_stage.cc
  mcc/engine/engine_stats.h
  mcc/engine/engine_state.h mcc/engine/engine_state.cc
# Shader
  mcc/shader/shader_events.h mcc/shader/shader_events.cc
  mcc/shader/shader.h mcc/shader/shader.cc
  mcc/shader/shader_pipeline.h
  mcc/shader/shader_document.h mcc/shader/shader_document.cc
  mcc/shader/shader_spec.h mcc/shader/shader_spec.cc
  mcc/shader/shader_spec_json.h mcc/shader/shader_spec_json.cc
  mcc/shader/shader_compiler.h mcc/shader/shader_compiler.cc
  mcc/shader/shader_linker.h mcc/shader/shader_linker.cc
  mcc/shader/source.h mcc/shader/source.cc
  mcc/shader/shader_source_resolver.h mcc/shader/shader_source_resolver.cc
# Window
  mcc/window/monitor_constants.h
  mcc/window/monitor_glfw.h
  mcc/window/monitor.h
  mcc/window/window_constants.h
  mcc/window/window_event.h
  mcc/window/window.h mcc/window/window.cc
  mcc/window/window_glfw.h mcc/window/window_glfw.cc
# Image
  mcc/image/image_constants.h
  mcc/image/image.h mcc/image/image.cc
  mcc/image/image_data.h mcc/image/image_data.cc
  mcc/image/image_decoder.h
  mcc/image/image_png.h mcc/image/image_png.cc
  mcc/image/image_jpeg.h mcc/image/image_jpeg.cc
# Textures
  mcc/texture/texture_factory.h mcc/texture/texture_factory.cc
  mcc/texture/texture_file.h
  mcc/texture/texture_event.h mcc/texture/texture_event.cc
  mcc/texture/texture_file_resolver.h
  mcc/texture/texture_spec.h mcc/texture/texture_spec.cc
  mcc/texture/texture_wrap.h
  mcc/texture/texture_target.h
  mcc/texture/texture_filter.h
  mcc/texture/texture_alignment.h
  mcc/texture/texture_constants.h
  mcc/texture/texture_property.h
  mcc/texture/texture_options.h mcc/texture/texture_options.cc
  mcc/texture/texture.h mcc/texture/texture.cc
# Terrain
  mcc/terrain/terrain_flags.h
  mcc/terrain/terrain.h mcc/terrain/terrain.cc
  mcc/terrain/terrain_vertex.h
  mcc/terrain/terrain_chunk.h mcc/terrain/terrain_chunk.cc
  mcc/murmur.h
  mcc/settings.h mcc/settings.cc
  mcc/bitfield.h
  mcc/light/point.h mcc/light/point.cc
  mcc/light/directional.h mcc/light/directional.cc
  mcc/lighting/ambient_light.h mcc/lighting/ambient_light.cc
  mcc/font/font.h mcc/font/font.cc
  mcc/font/glyph.h mcc/font/glyph.cc
  mcc/font/font_mesh.h mcc/font/font_mesh.cc
  mcc/font/truetype.h
  mcc/font/font_renderer.h mcc/font/font_renderer.cc
  mcc/camera/camera.h mcc/camera/camera.cc
  mcc/camera/perspective_camera.h mcc/camera/perspective_camera.cc
  mcc/ecs/entity.h mcc/ecs/entity.cc
# GUIs
  mcc/gui/shape.h mcc/gui/shape.cc
  mcc/gui/constants.h
  mcc/gui/window.h mcc/gui/window.cc
  mcc/gui/component.h
  mcc/gui/container.h
  mcc/gui/context.h mcc/gui/context.cc
  mcc/gui/gui.h mcc/gui/gui.cc
  mcc/gui/gui_viewport.h mcc/gui/gui_viewport.cc
  mcc/gui/gui_frame.h mcc/gui/gui_frame.cc
  mcc/gui/gui_frame_settings.h mcc/gui/gui_frame_settings.cc
  mcc/gui/gui_frame_renderer.h mcc/gui/gui_frame_renderer.cc
# Resource
  mcc/resource/resource_event.h mcc/resource/resource_event.cc
  mcc/resource/resource_type.h
  mcc/resource/resource_ptr.h
  mcc/resource/resource_loader.h
  mcc/resource/resource.h mcc/resource/resource.cc
# Misc
  mcc/event.h
  mcc/state_machine.h
  mcc/bitset.h
  mcc/uri.h mcc/uri.cc
  mcc/particle/particle.h mcc/particle/particle.cc
  mcc/particle/particles.h mcc/particle/particles.cc)
# add_dependencies(${PROJECT_NAME} fbschemas)
target_link_libraries(${PROJECT_NAME}
  Threads::Threads
  glog::glog
  ${GFLAGS_LIBRARIES}
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