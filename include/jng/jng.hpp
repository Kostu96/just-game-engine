/*
 * Copyright (C) 2020-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/base.hpp"
#include "core/engine.hpp"
#include "core/event.hpp"
#include "core/key_events.hpp"
#include "core/layer.hpp"
#include "core/mouse_events.hpp"
#include "core/window_events.hpp"

//#include "platform/graphics_context.hpp"
#include "platform/input.hpp"
#include "platform/key_codes.hpp"
#include "platform/mouse_codes.hpp"
#include "platform/window.hpp"

//#include "renderer/buffers.hpp"
#include "renderer/camera.hpp"
#include "renderer/renderer2d.hpp"
#include "renderer/renderer_api.hpp"
#include "renderer/shader.hpp"
//#include "renderer/shader_sources.hpp"
#include "renderer/texture.hpp"
//#include "renderer/vertex_array.hpp"

//#include "scene/entity.hpp"
//#include "scene/scene.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <imgui.h>

#if defined(JNG_DECLARE_MAIN)
#include "core/entry_point.hpp"
#endif
