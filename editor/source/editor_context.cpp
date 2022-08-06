/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "editor_context.hpp"

#include <jng/scripting/lua_engine.hpp>
#include <jng/serializers/scene_serializer.hpp>

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace jng {

    void EditorContext::createProject(std::filesystem::path path)
    {
        IsProjectOpen = true;

        ProjectPath = path;
        if (!std::filesystem::exists(ProjectPath))
            std::filesystem::create_directories(ProjectPath);

        std::filesystem::path filename = ProjectPath / ProjectPath.filename();
        filename += ".proj.yml";
        YAML::Emitter yaml;

        yaml << YAML::BeginMap;

        yaml << YAML::Key << "Project" << YAML::Value << ProjectPath.filename().string();

        yaml << YAML::EndMap;

        std::ofstream fout{ filename };
        fout << yaml.c_str();
        fout.close();

        AssetsPath = ProjectPath / "assets";
        if (!std::filesystem::exists(AssetsPath))
            std::filesystem::create_directories(AssetsPath);
        BrowsedPath = AssetsPath;
    }

    void EditorContext::openProject(std::filesystem::path path)
    {
        IsProjectOpen = true;

        ProjectPath = path;
        AssetsPath = ProjectPath / "assets";
        BrowsedPath = AssetsPath;

        for (auto& entry : std::filesystem::recursive_directory_iterator(BrowsedPath))
        {
            JNG_CORE_INFO("{}", entry);
            if (entry.is_regular_file() && entry.path().extension() == ".lua")
                LuaEngine::registerScript(entry.path());
        }
    }

    void EditorContext::createScene()
    {
        EditorScenePath = std::filesystem::path{};
        SelectedEntity = {};
        EditorScene = makeRef<Scene>();
        ActiveScene = EditorScene;

        ActiveScene->setViewportSize(ViewportWindowSize.x, ViewportWindowSize.y);
    }

    void EditorContext::openScene(std::filesystem::path path)
    {
        if (SceneState != SceneState::Stopped)
            onSceneStop();

        SelectedEntity = {};
        EditorScene = makeRef<Scene>();
        SceneSerializer serializer{ EditorScene };
        serializer.deserialize(path);
        EditorScenePath = path;

        EditorScene->setViewportSize(ViewportWindowSize.x, ViewportWindowSize.y);

        ActiveScene = EditorScene;
    }

    void EditorContext::saveScene(std::filesystem::path path)
    {
        if (!path.empty())
        {
            SceneSerializer serializer{ EditorScene };
            serializer.serialize(path);
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
