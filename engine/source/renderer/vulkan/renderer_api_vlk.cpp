/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/vulkan/renderer_api_vlk.hpp"

#include "renderer/vertex_array.hpp"
#include "renderer/buffers.hpp"

namespace jng {

    void VulkanRendererAPI::setViewport(uint32 /*x*/, uint32 /*y*/, uint32 /*width*/, uint32 /*height*/) const
    {
        
    }
    
    void VulkanRendererAPI::clear(const glm::vec3& /*color*/) const
    {
        
    }

    void VulkanRendererAPI::draw(uint32 /*count*/, RendererAPI::PrimitiveType /*primitiveType*/) const
    {
    }

    void VulkanRendererAPI::drawIndexed(uint32 /*count*/) const
    {
        
    }

} // namespace jng
