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
		SelectedEntity = {};
		ActiveScene = makeRef<Scene>();
		SceneSerializer serializer{ ActiveScene };
		serializer.deserialize(path.string().c_str());
	}

} // namespace jng
