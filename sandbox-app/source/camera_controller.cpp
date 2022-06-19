/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "camera_controller.hpp"

CameraController::CameraController(float aspectRatio) :
    m_aspectRatio(aspectRatio),
    m_zoomLevel(1.f),
    m_camera(-aspectRatio*2.f, aspectRatio*2.f, -2.f, 2.f),
    m_cameraPosition(0.f, 0.f),
    m_cameraMoveSpeed(2.f) {}

void CameraController::onUpdate(float dt)
{
    glm::vec2 move{};
    if (k2d::Input::isKeyPressed(k2d::Key::W))
        move.y = m_cameraMoveSpeed;
    else if (k2d::Input::isKeyPressed(k2d::Key::S))
        move.y = -m_cameraMoveSpeed;
    if (k2d::Input::isKeyPressed(k2d::Key::A))
        move.x = -m_cameraMoveSpeed;
    else if (k2d::Input::isKeyPressed(k2d::Key::D))
        move.x = m_cameraMoveSpeed;

    if (move.x != 0.f || move.y != 0)
    {
        move = glm::normalize(move);
        m_cameraPosition += move * dt * m_zoomLevel;
        m_camera.setPosition(m_cameraPosition);
    }
}

void CameraController::onEvent(k2d::Event& event)
{
    k2d::EventDispatcher dispatcher(event);
    dispatcher.dispatch<k2d::MouseScrollEvent>(K2D_BIND_EVENT_FUNC(CameraController::onMouseScroll));
    dispatcher.dispatch<k2d::WindowResizeEvent>(K2D_BIND_EVENT_FUNC(CameraController::onWindowResize));
}

bool CameraController::onMouseScroll(k2d::MouseScrollEvent& event)
{
    m_zoomLevel -= (event.getYOffset() * 0.05f);
    if (m_zoomLevel < 0.01f)
        m_zoomLevel = 0.01f;
    else if (m_zoomLevel > 10.f)
        m_zoomLevel = 10.f;

    m_camera.setProjection(
        -m_aspectRatio * m_zoomLevel * 2.f, m_aspectRatio * m_zoomLevel * 2.f,
        -m_zoomLevel * 2.f, m_zoomLevel * 2.f
    );
    
    return true;
}

bool CameraController::onWindowResize(k2d::WindowResizeEvent& event)
{
    m_aspectRatio = static_cast<float>(event.getWidth()) / static_cast<float>(event.getHeight());

    m_camera.setProjection(
        -m_aspectRatio * m_zoomLevel * 2.f, m_aspectRatio * m_zoomLevel * 2.f,
        -m_zoomLevel * 2.f, m_zoomLevel * 2.f
    );
    return false;
}
