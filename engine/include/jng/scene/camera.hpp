/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {

    class Camera
    {
    public:
        enum class ProjectionType { Orthographic = 0, Perspective = 1 };

        Camera() { recalculateProjection(); }
        Camera(const Camera&) = default;
        void reset() {} // TODO: impl

        void setOrthographic(float oSize, float oNear = -1.f, float oFar = 1.f);
        void setPerspective(float pFOV, float pNear = 0.01f, float pFar = 1000.f);
        const glm::mat4& getProjection() const { return m_projection; }

        void setProjectionType(ProjectionType type) { m_projectionType = type; recalculateProjection(); }
        ProjectionType getProjectionType() const { return m_projectionType; }

        void setViewportSize(float width, float height);
        float getAspectRatio() const { return m_aspectRatio; }

        float getOrthographicSize() const { return m_orthoSize; }
        float getOrthographicNear() const { return m_orthoNear; }
        float getOrthographicFar() const { return m_orthoFar; }
        void setOrthographicSize(float oSize) { m_orthoSize = oSize; recalculateProjection(); }
        void setOrthographicNear(float oNear) { m_orthoNear = oNear; recalculateProjection(); }
        void setOrthographicFar(float oFar) { m_orthoFar = oFar; recalculateProjection(); }

        float getPerspectiveFOV() const { return m_perspectiveFOV; }
        float getPerspectiveNear() const { return m_perspectiveNear; }
        float getPerspectiveFar() const { return m_perspectiveFar; }
        void setPerspectiveFOV(float pFOV) { m_perspectiveFOV = pFOV; recalculateProjection(); }
        void setPerspectiveNear(float pNear) { m_perspectiveNear = pNear; recalculateProjection(); }
        void setPerspectiveFar(float pFar) { m_perspectiveFar = pFar; recalculateProjection(); }

        glm::mat4 getVP(const glm::mat4 transform) const;
    private:
        void recalculateProjection();

        glm::mat4 m_projection{ 1.f };
        f32 m_aspectRatio{ 0.f };
        f32 m_orthoSize{ 10.f };
        f32 m_orthoNear{ -1.f };
        f32 m_orthoFar{ 1.f };
        f32 m_perspectiveFOV{ 45.f };
        f32 m_perspectiveNear{ 0.01f };
        f32 m_perspectiveFar{ 1000.f };
        ProjectionType m_projectionType = ProjectionType::Orthographic;
    };

} // namespace jng
