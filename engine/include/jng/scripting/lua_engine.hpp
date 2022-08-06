/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include <jng/core/base.hpp>

namespace jng::LuaEngine {
    
    void init();
    void shutdown();
    
    std::string registerScript(const std::filesystem::path& path);

} // namespace jng::LuaEngine
