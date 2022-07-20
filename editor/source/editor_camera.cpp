/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "editor_camera.hpp"

#include <jng/core/event.hpp>
#include <jng/core/mouse_events.hpp>
#include <jng/platform/input.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace jng {

    EditorCamera::EditorCamera()
    {
        setProjectionType(Camera::ProjectionType::Perspective);

        updateTransform();
    }

    void EditorCamera::onUpdate()
    {
        static constexpr float deltaFactor = 0.003f;
        static constexpr float rotationSpeed = 0.8f;
        static glm::vec2 initialPos{ 0.f, 0.f };

        glm::vec2 mouse = Input::getMousePosition();
        glm::vec2 delta = (mouse - initialPos) * deltaFactor;
        initialPos = mouse;

        if (Input::isMouseButtonPressed(Mouse::Right))
        {
            float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
            m_yaw += yawSign * delta.x * rotationSpeed;
            m_pitch += delta.y * rotationSpeed;
        }
        else if (Input::isMouseButtonPressed(Mouse::Middle))
        {
            glm::vec2 speed = panSpeed();
            m_focalPoint += -getRightDirection() * delta.x * speed.x * m_distance;
            m_focalPoint += getUpDirection() * delta.y * speed.y * m_distance;
        }

        updateTransform();
    }

    void EditorCamera::onEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<MouseScrollEvent>(JNG_BIND_EVENT_FUNC(EditorCamera::OnMouseScroll));
    }

    void EditorCamera::setViewportSize(uint32 width, uint32 height)
    {
        Camera::setViewportSize(width, height);
        m_viewportWidth = static_cast<float>(width);
        m_viewportHeight = static_cast<float>(height);
    }

    bool EditorCamera::OnMouseScroll(MouseScrollEvent& event)
    {
        float delta = event.getYOffset() * 0.1f;
        m_distance -= delta * zoomSpeed();
        if (m_distance < 1.0f)
        {
            m_focalPoint += getForwardDirection();
            m_distance = 1.0f;
        }

        return false;
    }

    void EditorCamera::updateTransform()
    {
        m_position = m_focalPoint - getForwardDirection() * m_distance;

        glm::quat orientation = getOrientation();
        m_transform = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
    }

    glm::quat EditorCamera::getOrientation() const
    {
        return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
    }

    glm::vec3 EditorCamera::getForwardDirection() const
    {
        return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 EditorCamera::getUpDirection() const
    {
        return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::getRightDirection() const
    {
        return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec2 EditorCamera::panSpeed() const
    {
        // CREDIT: this quadratic equation coefficients come from Yan Chernikov (The Cherno) EditorCamera implementation
        static constexpr float a = 0.0366f;
        static constexpr float b = 0.1778f;
        static constexpr float c = 0.3021f;

        float x = std::min(m_viewportWidth / 1000.0f, 2.4f);
        float y = std::min(m_viewportHeight / 1000.0f, 2.4f);

        return { 
            a * (x * x) - b * x + c,
            a * (y * y) - b * y + c
        };
    }

    float EditorCamera::zoomSpeed() const
    {
        // CREDIT: this zoom speed calculation comes from Yan Chernikov (The Cherno) EditorCamera implementation
        float distance = m_distance * 0.2f;
        distance = std::max(distance, 0.0f);
        float speed = distance * distance;
        speed = std::min(speed, 100.0f);
        return speed;
    }

} // namespace jng
