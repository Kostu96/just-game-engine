/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/scene/entity.hpp>

namespace jng {

	class NativeScript
	{
	public:
		explicit NativeScript() = default;
		virtual ~NativeScript() = default;

		virtual void onCreate() {}
		virtual void onDestroy() {}
		virtual void onUpdate(float /*dt*/) {}
	protected:
		template<typename T>
		T& getComponent() { return m_entity.getComponent<T>(); }
	private:
		Entity m_entity;
	};

} // namespace jng
