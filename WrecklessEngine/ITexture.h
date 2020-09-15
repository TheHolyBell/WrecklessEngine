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

        virtual unsigned GetWidth() const PURE;
        virtual unsigned GetHeight() const PURE;

        virtual ~ITexture() = default;
    };

    class IRenderTarget : public ITexture
    {
    public:
        IRenderTarget() = default;

        IRenderTarget(const IRenderTarget& rhs) = delete;
        IRenderTarget& operator=(const IRenderTarget& rhs) = delete;

        IRenderTarget(IRenderTarget&& rhs) = delete;
        IRenderTarget& operator=(IRenderTarget&& rhs) = delete;

        virtual ~IRenderTarget() = default;
    };

    class IDepthStencilView : public ITexture
    {
    public:
        IDepthStencilView() = default;

        IDepthStencilView(const IDepthStencilView& rhs) = delete;
        IDepthStencilView& operator=(const IDepthStencilView& rhs) = delete;
    
        IDepthStencilView(IDepthStencilView&& rhs) = delete;
        IDepthStencilView& operator=(IDepthStencilView&& rhs) = delete;
    
        virtual ~IDepthStencilView() = default;
    };
}