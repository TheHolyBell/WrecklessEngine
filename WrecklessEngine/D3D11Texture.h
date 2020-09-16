#pragma once
#include "CommonInclude.h"
#include "ITexture.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Graphics
{
	class D3D11Texture : public ITexture
	{
	public:
		D3D11Texture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);

		virtual TEX_FORMAT GetFormat() const override;
		virtual void* GetNativePointer() const override;

		virtual unsigned GetWidth() const override;
		virtual unsigned GetHeight() const override;

		virtual ~D3D11Texture();
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pView;
		TEX_FORMAT m_Format;
		unsigned m_Width = 0;
		unsigned m_Height = 0;
	};

	class D3D11RenderTarget : public IRenderTarget
	{
	public:
		D3D11RenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  pRTV);

		virtual TEX_FORMAT GetFormat() const override;

		virtual void* GetNativePointer() const override;

		virtual unsigned GetWidth() const override;
		virtual unsigned GetHeight() const override;

		unsigned long Reset();

		virtual ~D3D11RenderTarget();

	private:
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV;
		TEX_FORMAT m_Format;
		unsigned m_Width = 0;
		unsigned m_Height = 0;
	};

	class D3D11DepthStencilView : public IDepthStencilView
	{
	public:
		D3D11DepthStencilView(Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSV);

		virtual TEX_FORMAT GetFormat() const override;

		virtual void* GetNativePointer() const override;

		virtual unsigned GetWidth() const override;
		virtual unsigned GetHeight() const override;

		unsigned long Reset();

		virtual ~D3D11DepthStencilView();

	private:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;
		TEX_FORMAT m_Format;
		unsigned m_Width = 0;
		unsigned m_Height = 0;
	};
}