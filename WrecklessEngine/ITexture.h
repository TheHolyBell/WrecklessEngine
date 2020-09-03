#pragma once
#include "CommonInclude.h"
#include "GraphicsFormats.h"

namespace Graphics
{
    

    class ITexture
    {
    public:
        ITexture() = default;

        ITexture(const ITexture& rhs) = delete;
        ITexture& operator=(const ITexture& rhs) = delete;

        ITexture(ITexture&& rhs) = delete;
        ITexture& operator=(ITexture&& rhs) = delete;

        virtual TEX_FORMAT GetFormat() const PURE;
        virtual void* GetNativePointer() const PURE;

        virtual ~ITexture() = default;
    };
}