/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/jng.hpp>

class CameraController
{
public:
    CameraController(float aspectRatio);

    void onUpdate(float dt);
    void onEvent(jng::Event& event);

    const jng::Camera& getCamera() const { return m_camera; }
private:
    bool onMouseScroll(jng::MouseScrollEvent& event);
    bool onWindowResize(jng::WindowResizeEvent& event);

    float m_aspectRatio;
    float m_zoomLevel;
    jng::Camera m_camera;
    glm::vec2 m_cameraPosition;
    float m_cameraMoveSpeed;
};
