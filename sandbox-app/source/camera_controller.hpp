/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <k2d/k2d.hpp>

class CameraController
{
public:
    CameraController(float aspectRatio);

    void onUpdate(float dt);
    void onEvent(k2d::Event& event);

    const k2d::Camera& getCamera() const { return m_camera; }
private:
    bool onMouseScroll(k2d::MouseScrollEvent& event);
    bool onWindowResize(k2d::WindowResizeEvent& event);

    float m_aspectRatio;
    float m_zoomLevel;
    k2d::Camera m_camera;
    glm::vec2 m_cameraPosition;
    float m_cameraMoveSpeed;
};
