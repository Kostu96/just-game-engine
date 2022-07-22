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
		JNG_USER_WARN(path.string().c_str());
		SelectedEntity = {};
		ActiveScene = makeRef<Scene>();
		SceneSerializer serializer{ ActiveScene };
		serializer.deserialize(path.string().c_str());

		ActiveScene->setViewportSize(ViewportWindowSize.x, ViewportWindowSize.y);
	}

} // namespace jng
