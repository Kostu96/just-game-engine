/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "scene/camera.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace jng {

    void Camera::setOrthographic(f32 oSize, f32 oNear, f32 oFar)
    {
        m_projectionType = ProjectionType::Orthographic;
        m_orthoSize = oSize;
        m_orthoNear = oNear;
        m_orthoFar = oFar;
        recalculateProjection();
    }

    void Camera::setPerspective(f32 pFOV, f32 pNear, f32 pFar)
    {
        m_projectionType = ProjectionType::Perspective;
        m_perspectiveFOV = pFOV;
        m_perspectiveNear = pNear;
        m_perspectiveFar = pFar;
        recalculateProjection();
    }

    void Camera::setViewportSize(f32 width, f32 height)
    {
        JNG_CORE_ASSERT(width > 0.f && height > 0.f, "Viewport area can't be 0!");
        m_aspectRatio = width / height;
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
            f32 h = m_orthoSize * m_aspectRatio * .5f;
            f32 v = m_orthoSize * .5f;
            m_projection = glm::orthoRH_ZO(-h, h, -v, v, m_orthoNear, m_orthoFar);
        } break;
        case ProjectionType::Perspective:
            m_projection = glm::perspectiveRH_ZO(glm::radians(m_perspectiveFOV), m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
            break;
        }
    }

} // namespace jng
