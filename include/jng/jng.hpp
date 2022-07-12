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

#include "platform/input.hpp"
#include "platform/key_codes.hpp"
#include "platform/mouse_codes.hpp"
#include "platform/window.hpp"

#include "renderer/buffers.hpp"
#include "renderer/camera.hpp"
#include "renderer/framebuffer.hpp"
#include "renderer/renderer.hpp"
#include "renderer/renderer_api.hpp"
#include "renderer/renderer2d.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/vertex_array.hpp"

#include "scene/components.hpp"
#include "scene/entity.hpp"
#include "scene/scene.hpp"

#include "scripting/native_script.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>
#include <imgui.h>

#if defined(JNG_DECLARE_MAIN)
#include "core/entry_point.hpp"
#endif
