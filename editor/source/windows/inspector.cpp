/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "inspector.hpp"

#include "../editor_layer.hpp"

#include <imgui.h>

namespace jng {

	void InspectorWindow::onImGuiUpdate()
	{
		if (m_context.isInspectorWindowOpen)
		{
			ImGui::Begin("Inspector", &m_context.isInspectorWindowOpen, ImGuiWindowFlags_NoCollapse);

			ImGui::End();
		}
	}

} // namespace jng
