/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/camera.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace jng {

    void Camera::setOrthographic(float size, float near, float far)
    {
        m_projectionType = ProjectionType::Orthographic;
        m_orthoSize = size;
        m_orthoNear = near;
        m_orthoFar = far;
        recalculateProjection();
    }

    void Camera::setPerspective(float fov, float near, float far)
    {
        m_projectionType = ProjectionType::Perspective;
        m_perspectiveFOV = fov;
        m_perspectiveNear = near;
        m_perspectiveFar = far;
        recalculateProjection();
    }

    void Camera::setViewportSize(uint32 width, uint32 height)
    {
        JNG_CORE_ASSERT(width > 0 && height > 0, "Viewport area can't be 0!");
        m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        recalculateProjection();
    }

    glm::mat4 Camera::getVP(const glm::mat4 transform) const
    {
        JNG_PROFILE_FUNCTION();

        return m_projection * glm::inverse(transform);
    }

    void Camera::recalculateProjection()
    {
        JNG_PROFILE_FUNCTION();

        switch (m_projectionType)
        {
        case ProjectionType::Orthographic:
        {
            float h = m_orthoSize * m_aspectRatio * .5f;
            float v = m_orthoSize * .5f;
            m_projection = glm::orthoRH_ZO(-h, h, -v, v, m_orthoNear, m_orthoFar);
        } break;
        case ProjectionType::Perspective:
            m_projection = glm::perspectiveRH_ZO(glm::radians(m_perspectiveFOV), m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
            break;
        }
    }

} // namespace jng
