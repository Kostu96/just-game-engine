/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "editor_context.hpp"

#include <jng/serializers/scene_serializer.hpp>

namespace jng {

    void EditorContext::openScene(std::filesystem::path path)
    {
        if (SceneState != SceneState::Stopped)
            onSceneStop();

        SelectedEntity = {};
        EditorScene = makeRef<Scene>();
        SceneSerializer serializer{ EditorScene };
        serializer.deserialize(path.string().c_str());
        EditorScenePath = path;

        EditorScene->setViewportSize(ViewportWindowSize.x, ViewportWindowSize.y);

        ActiveScene = EditorScene;
    }

    void EditorContext::saveScene(std::filesystem::path path)
    {
        if (!path.empty())
        {
            SceneSerializer serializer{ EditorScene };
            serializer.serialize(path.string().c_str());
            EditorScenePath = path;
        }
    }

    void EditorContext::onSceneStart()
    {
        SceneState = SceneState::Playing;
        
        ActiveScene = Scene::copy(EditorScene);
        ActiveScene->onCreate(physicsGravity);
    }

    void EditorContext::onSceneStop()
    {
        SceneState = SceneState::Stopped;

        ActiveScene->onDestroy();
        ActiveScene = EditorScene;
    }

} // namespace jng
