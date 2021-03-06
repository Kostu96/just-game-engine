/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/shader.hpp"
#include "platform/windows/windows_base.hpp"
#include "platform/windows/graphics_context_d3d.hpp"

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
        Direct3DShader(std::string_view vertexShaderFilename, std::string_view fragmentShaderFilename);
        ~Direct3DShader();

        void bind() const override;
        void unbind() const override {}

        ID3DBlob* getVertexShaderByteCode() const { return m_vertexShaderByteCode.Get(); }
    protected:
        std::filesystem::path getCacheDirectory() const override;
    private:
        void compileShader(const char* shaderFilename, Type type, wrl::ComPtr<ID3DBlob>& byteCode);

        static const char* shaderTypeToCachedD3DFileExtension(Type type);
        static const char* shaderTypeToD3DTarget(Type type);

        const Direct3DGraphicsContext* m_graphicsContext;
        wrl::ComPtr<ID3DBlob> m_vertexShaderByteCode;
        wrl::ComPtr<ID3D11VertexShader> m_vertexShader;
        wrl::ComPtr<ID3D11PixelShader> m_fragmentShader;
        mutable std::vector<std::string> m_bufferLookup;
    };

} // namespace jng
