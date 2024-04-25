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
  mcc/aabb.h mcc/aabb.cc
  mcc/transform.h mcc/transform.cc
  mcc/material/material.h mcc/material/material.cc
  mcc/material/material_json.h mcc/material/material_json.cc
  mcc/material/material_loader.h mcc/material/material_loader.cc
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
  mcc/subscription_trie.h
# Components
  mcc/component/component_id.h
  mcc/component/component_events.h mcc/component/component_events.cc
  mcc/component/component_state.h
  mcc/component/component.h mcc/component/component.cc
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
  mcc/renderer/render_events.h mcc/renderer/render_events.cc
  mcc/renderer/projection_matrix.h mcc/renderer/projection_matrix.cc
  mcc/renderer/render_pass.h mcc/renderer/render_pass.cc
  mcc/renderer/render_pass_2d.h mcc/renderer/render_pass_2d.cc
  mcc/renderer/render_pass_3d.h mcc/renderer/render_pass_3d.cc
  mcc/renderer/render_pass_stats.h mcc/renderer/render_pass_stats.cc
  mcc/renderer/render_pass_executor.h mcc/renderer/render_pass_executor.cc
  mcc/renderer/renderer_state.h
  mcc/renderer/renderer_stats.h
  mcc/renderer/renderer.h mcc/renderer/renderer.cc
  mcc/renderer/render_ticker.h mcc/renderer/render_ticker.cc
# Engine
  mcc/engine/engine_event.h mcc/engine/engine_event.cc
  mcc/engine/engine.h mcc/engine/engine.cc
  mcc/engine/engine_stats.h
  mcc/engine/engine_state.h
  mcc/engine/engine_state_init.h mcc/engine/engine_state_init.cc
  mcc/engine/engine_state_tick.h mcc/engine/engine_state_tick.cc
  mcc/engine/engine_state_terminated.h mcc/engine/engine_state_terminated.cc
  mcc/engine/engine_ticker.h mcc/engine/engine_ticker.cc
# VBOs
  mcc/vbo/vbo_id.h mcc/vbo/vbo_id.cc
  mcc/vbo/vbo_events.h mcc/vbo/vbo_events.cc
  mcc/vbo/vbo_builder.h mcc/vbo/vbo_builder.cc
  mcc/vbo/vbo_scope.h mcc/vbo/vbo_scope.cc
  mcc/vbo/vbo.h mcc/vbo/vbo.cc
  mcc/vbo/vbo_layout.h
  mcc/vbo/vbo_attribute.h
# IBOs
  mcc/ibo/ibo_id.h
  mcc/ibo/ibo_index.h
  mcc/ibo/ibo_usage.h
  mcc/ibo/ibo_events.h mcc/ibo/ibo_events.cc
  mcc/ibo/ibo.h mcc/ibo/ibo.cc
  mcc/ibo/ibo_factory.h mcc/ibo/ibo_factory.cc
  mcc/ibo/ibo_scope.h
  mcc/ibo/ibo_registry.h mcc/ibo/ibo_registry.cc
# VAOs
  mcc/vao/vao.h mcc/vao/vao.cc
  mcc/vao/vao_id.h
  mcc/vao/vao_scope.h
  mcc/vao/vao_events.h mcc/vao/vao_events.cc
# Program
  mcc/program/program_document.h
  mcc/program/program_events.h mcc/program/program_events.cc
  mcc/program/program_id.h
  mcc/program/program_linker.h mcc/program/program_linker.cc
  mcc/program/program_pipeline.h mcc/program/program_pipeline.cc
  mcc/program/program_spec_builder.h mcc/program/program_spec_builder.cc
  mcc/program/program_spec_json.h mcc/program/program_spec_json.cc
  mcc/program/program_spec.h mcc/program/program_spec.cc
  mcc/program/program_status.h mcc/program/program_status.cc
  mcc/program/program.h mcc/program/program.cc
# Shader
  mcc/shader/shader_id.h
  mcc/shader/shader_type.h
  mcc/shader/shader_code.h mcc/shader/shader_code.cc
  mcc/shader/shader_events.h mcc/shader/shader_events.cc
  mcc/shader/shader.h mcc/shader/shader.cc
  mcc/shader/shader_status.h
  mcc/shader/shader_document.h mcc/shader/shader_document.cc
  mcc/shader/shader_spec.h mcc/shader/shader_spec.cc
  mcc/shader/shader_spec_json.h mcc/shader/shader_spec_json.cc
  mcc/shader/shader_compiler.h mcc/shader/shader_compiler.cc
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
  mcc/texture/texture_id.h
  mcc/texture/texture_factory.h mcc/texture/texture_factory.cc
  mcc/texture/texture_file.h
  mcc/texture/texture_document.h mcc/texture/texture_document.cc
  mcc/texture/texture_event.h mcc/texture/texture_event.cc
  mcc/texture/texture_spec.h mcc/texture/texture_spec.cc
  mcc/texture/texture_spec_builder.h mcc/texture/texture_spec_builder.cc
  mcc/texture/texture_spec_json.h mcc/texture/texture_spec_json.cc
  mcc/texture/texture_wrap.h
  mcc/texture/texture_target.h
  mcc/texture/texture_filter.h
  mcc/texture/texture_alignment.h
  mcc/texture/texture_constants.h
  mcc/texture/texture_property.h
  mcc/texture/texture_options.h mcc/texture/texture_options.cc
  mcc/texture/texture.h mcc/texture/texture.cc
# RBOs
  mcc/rbo/rbo_events.h mcc/rbo/rbo_events.cc
  mcc/rbo/rbo_factory.h mcc/rbo/rbo_factory.cc
  mcc/rbo/rbo_id.h
  mcc/rbo/rbo.h mcc/rbo/rbo.cc
  mcc/rbo/rbo_format.h
  mcc/rbo/rbo_size.h mcc/rbo/rbo_size.cc
# FBOs
  mcc/fbo/fbo_events.h mcc/fbo/fbo_events.cc
  mcc/fbo/fbo_factory.h mcc/fbo/fbo_factory.cc
  mcc/fbo/fbo_id.h
  mcc/fbo/fbo_status.h
  mcc/fbo/fbo_target.h
  mcc/fbo/fbo_pipeline.h mcc/fbo/fbo_pipeline.cc
  mcc/fbo/fbo_attachments.h mcc/fbo/fbo_attachments.cc
  mcc/fbo/fbo_attachment.h
  mcc/fbo/fbo_attachment_color.h mcc/fbo/fbo_attachment_color.cc
  mcc/fbo/fbo_attachment_depth.h mcc/fbo/fbo_attachment_depth.cc
  mcc/fbo/fbo_attachment_stencil.h mcc/fbo/fbo_attachment_stencil.cc
  mcc/fbo/fbo.h mcc/fbo/fbo.cc
# Camera
  mcc/camera/camera.h mcc/camera/camera.cc
  mcc/camera/perspective_camera.h mcc/camera/perspective_camera.cc
  mcc/camera/perspective_camera_component.h mcc/camera/perspective_camera_component.cc
# Entity
  mcc/entity/generation.h mcc/entity/generation.cc
  mcc/entity/entity_id.h
  mcc/entity/entity_signature.h
  mcc/entity/entity.h mcc/entity/entity.cc
  mcc/entity/entity_events.h mcc/entity/entity_events.cc
  mcc/entity/entity_tracker.h mcc/entity/entity_tracker.cc
# GUI
# Resource
  mcc/resource/resource_event.h mcc/resource/resource_event.cc
  mcc/resource/resource_type.h
  mcc/resource/resource_ptr.h
  mcc/resource/resource_loader.h
  mcc/resource/resource.h mcc/resource/resource.cc
# libuv extensions
  mcc/uv/utils.h
  mcc/uv/uv_tick.h
  mcc/uv/uv_ticker.h
  mcc/uv/uv_status.h
  mcc/uv/uv_handle.h
  mcc/uv/uv_handle_async.h
  mcc/uv/uv_handle_check.h
  mcc/uv/uv_handle_idle.h
  mcc/uv/uv_handle_prepare.h
# Font
  mcc/font/font.h mcc/font/font.cc
  mcc/font/glyph.h mcc/font/glyph.cc
  mcc/font/font_mesh.h mcc/font/font_mesh.cc
  mcc/font/truetype.h
  mcc/font/font_renderer.h mcc/font/font_renderer.cc
# Misc
  mcc/murmur.h
  mcc/settings.h mcc/settings.cc
  mcc/bitfield.h
  mcc/uv.h
  mcc/on_shutdown.h
  mcc/event.h
  mcc/state_machine.h
  mcc/bitset.h
  mcc/uri.h mcc/uri.cc)
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