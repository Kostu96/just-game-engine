/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/shader.hpp"
#include "platform/windows/windows_base.hpp"
#include "platform/graphics_context_d3d.hpp"

#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;

namespace jng {

    class ShaderImpl
    {
    public:
        ShaderImpl(std::string_view vertexShaderSrc, std::string_view fragmentShaderSrc);
        ~ShaderImpl();

        void bind() const;
        void unbind() const;

        void set(const char* name, int value) const;
        void set(const char* name, const int* value, uint32 count) const;
        void set(const char* name, const glm::vec3& value) const;
        void set(const char* name, const glm::vec4& value) const;
        void set(const char* name, const glm::mat4& value) const;

        wrl::ComPtr<ID3DBlob>& getVertexShaderByteCode() { return m_vertexShaderByteCode; }
    private:
        const Direct3DGraphicsContext& m_graphicsContext;
        wrl::ComPtr<ID3DBlob> m_vertexShaderByteCode;
        wrl::ComPtr<ID3D11VertexShader> m_vertexShader;
        wrl::ComPtr<ID3D11PixelShader> m_fragmentShader;
    };

} // namespace jng
