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

        void onUpdate();
        void onEvent(Event& event);

        void setViewportSize(uint32 width, uint32 height);

        glm::mat4 getVP() const { return Camera::getVP(m_transform); }
    private:
        bool OnMouseScroll(MouseScrollEvent& event);

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
