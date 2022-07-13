/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core//base.hpp"

#include <glm/glm.hpp>

namespace jng {

    // TODO: decide if move to scene folder
    class Camera
    {
    public:
        enum class ProjectionType { Orthographic = 0, Perspective = 1 };

        Camera() { recalculateProjection(); }

        void setOrthographic(float size, float near = -1.f, float far = 1.f);
        void setPerspective(float fov, float near = 0.01f, float far = 1000.f);
        const glm::mat4& getProjection() const { return m_projection; }

        void setProjectionType(ProjectionType type) { m_projectionType = type; recalculateProjection(); }
        ProjectionType getProjectionType() const { return m_projectionType; }

        void setViewportSize(uint32 width, uint32 height);
        float getAspectRatio() const { return m_aspectRatio; }

        float getOrthographicSize() const { return m_orthoSize; }
        float getOrthographicNear() const { return m_orthoNear; }
        float getOrthographicFar() const { return m_orthoFar; }
        void setOrthographicSize(float size) { m_orthoSize = size; recalculateProjection(); }
        void setOrthographicNear(float near) { m_orthoNear = near; recalculateProjection(); }
        void setOrthographicFar(float far) { m_orthoFar = far; recalculateProjection(); }

        float getPerspectiveFOV() const { return m_perspectiveFOV; }
        float getPerspectiveNear() const { return m_perspectiveNear; }
        float getPerspectiveFar() const { return m_perspectiveFar; }
        void setPerspectiveFOV(float fov) { m_perspectiveFOV = fov; recalculateProjection(); }
        void setPerspectiveNear(float near) { m_perspectiveNear = near; recalculateProjection(); }
        void setPerspectiveFar(float far) { m_perspectiveFar = far; recalculateProjection(); }

        glm::mat4 getVP(const glm::mat4 transform) const;
    private:
        void recalculateProjection();

        glm::mat4 m_projection{ 1.f };
        ProjectionType m_projectionType = ProjectionType::Orthographic;
        float m_aspectRatio{ 0.f };
        float m_orthoSize{ 10.f };
        float m_orthoNear{ -1.f };
        float m_orthoFar{ 1.f };
        float m_perspectiveFOV{ 45.f };
        float m_perspectiveNear{ 0.01f };
        float m_perspectiveFar{ 1000.f };
    };

} // namespace jng
