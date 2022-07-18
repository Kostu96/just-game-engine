/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/scene/entity.hpp>

namespace jng {

	class Event;
	class Scene;

	class NativeScript
	{
	public:
		explicit NativeScript() = default;
		virtual ~NativeScript() = default;

		virtual void onCreate() {}
		virtual void onDestroy() {}
		virtual void onUpdate(float /*dt*/) {}
		virtual void onEvent(Event& /*event*/) {}
	protected:
		template<typename T>
		T& getComponent() { return m_entity.getComponent<T>(); }

		Entity createEntity(const std::string& name) { return m_entity.getScene()->createEntity(name); }
	private:
		Entity m_entity;

		friend class Scene;
	};

} // namespace jng
