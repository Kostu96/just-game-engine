/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <glm/glm.hpp>

namespace jng {

    class Camera
    {
    public:
        Camera(const glm::mat4& projection);

        void setProjection(const glm::mat4& projection);
        const glm::mat4& getProjection() const { return m_projection; }
        const glm::mat4& getView() const { return m_view; }
        const glm::mat4& getVP() const { return m_vp; }
        void setPosition(glm::vec2 position) { m_position = position; recalculateVP(); }
        glm::vec2 getPosition() const { return m_position; }
    private:
        void recalculateVP();

        glm::mat4 m_projection;
        glm::mat4 m_view;
        glm::mat4 m_vp;
        glm::vec2 m_position;
    };

    class OrthographicCamera :
        public Camera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        void setProjection(float left, float right, float bottom, float top);
    };

    class PerspectiveCamera :
        public Camera
    {
    public:
        PerspectiveCamera(float fov, float aspect, float near, float far);

        void setProjection(float fov, float aspect, float near, float far);
    };

} // namespace jng
