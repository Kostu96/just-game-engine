/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/camera.hpp"

#include "core/base_internal.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace jng {

    Camera::Camera(float left, float right, float bottom, float top) :
        m_projection(glm::ortho(left, right, bottom, top)),
        m_view(1.f),
        m_vp(m_projection * m_view),
        m_position(0.f, 0.f) {}

    void Camera::setProjection(float left, float right, float bottom, float top)
    {
        JNG_PROFILE_FUNCTION();

        m_projection = glm::ortho(left, right, bottom, top);
        m_vp = m_projection * m_view;
    }

    void Camera::recalculateViewAndVP()
    {
        JNG_PROFILE_FUNCTION();
        
        m_view = glm::inverse(glm::translate(glm::mat4{ 1.f }, glm::vec3{ m_position, 0.f }));

        m_vp = m_projection * m_view;
    }

} // namespace jng
