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

    class Direct3DShader :
        public Shader
    {
    public:
        Direct3DShader(std::string_view vertexShaderSrc, std::string_view fragmentShaderSrc);
        ~Direct3DShader();

        void bind() const override;
        void unbind() const override;

        void set(const char* name, int value) const override;
        void set(const char* name, const int* value, uint32 count) const override;
        void set(const char* name, const glm::vec3& value) const override;
        void set(const char* name, const glm::vec4& value) const override;
        void set(const char* name, const glm::mat4& value) const override;

        ID3DBlob* getVertexShaderByteCode() const { return m_vertexShaderByteCode.Get(); }
    private:
        const Direct3DGraphicsContext* m_graphicsContext;
        wrl::ComPtr<ID3DBlob> m_vertexShaderByteCode;
        wrl::ComPtr<ID3D11VertexShader> m_vertexShader;
        wrl::ComPtr<ID3D11PixelShader> m_fragmentShader;
        mutable std::vector<std::string> m_bufferLookup;
    };

} // namespace jng
