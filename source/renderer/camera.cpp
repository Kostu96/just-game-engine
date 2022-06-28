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

    Camera::Camera(const glm::mat4& projection) :
        m_projection(projection),
        m_view(1.f),
        m_vp(m_projection * m_view),
        m_position(0.f, 0.f) {}

    void Camera::setProjection(const glm::mat4& projection)
    {
        JNG_PROFILE_FUNCTION();

        m_projection = projection;
        m_vp = m_projection * m_view;
    }

    void Camera::recalculateVP()
    {
        JNG_PROFILE_FUNCTION();
        
        m_view = glm::inverse(glm::translate(glm::mat4{ 1.f }, glm::vec3{ m_position, 0.f }));

        m_vp = m_projection * m_view;
    }

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) :
        Camera{ RendererAPI::getRendererBackend() == RendererBackend::Direct3D ? 
        glm::orthoRH_ZO(left, right, bottom, top, -1.f, 1.f) :
        glm::orthoRH_NO(left, right, bottom, top, -1.f, 1.f) }
    {
    }

    void OrthographicCamera::setProjection(float left, float right, float bottom, float top)
    {
        switch (RendererAPI::getRendererBackend())
        {
        case RendererBackend::Direct3D:
            Camera::setProjection(glm::orthoRH_ZO(left, right, bottom, top, -1.f, 1.f));
            break;
        default:
            Camera::setProjection(glm::orthoRH_NO(left, right, bottom, top, -1.f, 1.f));
        }
    }

    PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far) :
        Camera{ RendererAPI::getRendererBackend() == RendererBackend::Direct3D ?
                glm::perspectiveRH_ZO(glm::radians(fov), aspect, near, far) :
                glm::perspectiveRH_NO(glm::radians(fov), aspect, near, far) }
    {
    }

    void PerspectiveCamera::setProjection(float fov, float aspect, float near, float far)
    {
        switch (RendererAPI::getRendererBackend())
        {
        case RendererBackend::Direct3D:
            Camera::setProjection(glm::perspectiveRH_ZO(glm::radians(fov), aspect, near, far));
            break;
        default:
            Camera::setProjection(glm::perspectiveRH_NO(glm::radians(fov), aspect, near, far));
        }
    }

} // namespace jng
