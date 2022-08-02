/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/scene/camera.hpp>

namespace jng {

    class Event;
    class MouseScrollEvent;

    class EditorCamera :
        public Camera
    {
    public:
        EditorCamera();
        void reset();

        void onUpdate();
        void onEvent(Event& event);

        void setViewportSize(float width, float height);

        glm::mat4 getView() const { return glm::inverse(m_transform); }
        glm::mat4 getViewProjection() const { return Camera::getVP(m_transform); }
    private:
        bool onMouseScroll(MouseScrollEvent& event);

        void updateTransform();
        glm::quat getOrientation() const;
        glm::vec3 getForwardDirection() const;
        glm::vec3 getUpDirection() const;
        glm::vec3 getRightDirection() const;
        glm::vec2 panSpeed() const;
        float zoomSpeed() const;

        glm::mat4 m_transform{ 1.f };
        glm::vec3 m_position{ 0.f, 0.f, 0.f };
        glm::vec3 m_focalPoint{ 0.f, 0.f, 0.f };
        float m_distance = 10.f;
        float m_pitch = 0.0f, m_yaw = 0.0f;
        float m_viewportWidth = 0.f;
        float m_viewportHeight = 0.f;
    };

} // namespace jng
