#include "Renderer.h"
#include "dxerr.h"
#include <sstream>
#include "DXGIInfoManager.h"
#include <d3d11.h>
#include <wrl/client.h>

#include "D3D11Device.h"
#include "D3D11RenderContext.h"
#include "D3D11SwapChain.h"

#include "StringHelper.h"

namespace Graphics
{
	Ref<IDevice> Renderer::s_Device;
	Ref<IRenderContext> Renderer::s_RenderContext;
	Ref<InfoManager> Renderer::s_InfoManager;
	Ref<ISwapChain> Renderer::s_SwapChain;
	RendererAPICapabilities Renderer::s_Capabilities;

	RendererAPICapabilities& Renderer::GetCapabilities()
	{
		return s_Capabilities;
	}

	void Renderer::Initialize(RendereringAPI apiType, IWindow& window)
	{
		s_InfoManager = std::make_shared<DXGIInfoManager>();

		switch (apiType)
		{
		case RendereringAPI::DirectX11:
			InitializeD3D11(window);
			break;
		case RendereringAPI::OpenGL:
			InitializeOpenGL(window);
			break;
		}
	}
	std::shared_ptr<IDevice> Renderer::GetDevice()
	{
		return s_Device;
	}
	std::shared_ptr<IRenderContext> Renderer::GetRenderContext()
	{
		return s_RenderContext;
	}
	Ref<ISwapChain> Renderer::GetSwapChain()
	{
		return s_SwapChain;
	}
	std::shared_ptr<InfoManager> Renderer::GetInfoManager()
	{
		return s_InfoManager;
	}

	void Renderer::InitializeD3D11(IWindow& window)
	{
		Microsoft::WRL::ComPtr<ID3D11Device> _device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> _swapChain;

		DXGI_SWAP_CHAIN_DESC _scd = {};
		_scd.BufferDesc.Width = window.GetWidth();
		_scd.BufferDesc.Height = window.GetHeight();
		_scd.BufferDesc.RefreshRate.Numerator = 60;
		_scd.BufferDesc.RefreshRate.Denominator = 1;
		_scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		_scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		_scd.BufferCount = 1;
		_scd.OutputWindow = (HWND)window.GetWindowHandle();
		_scd.Windowed = true;
		_scd.SampleDesc.Count = 1;
		_scd.SampleDesc.Quality = 0;
		_scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		WRECK_HR(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
			D3D11_SDK_VERSION, &_scd, &_swapChain, &_device, nullptr, &_deviceContext));

		if (_swapChain == nullptr || _device == nullptr || _deviceContext == nullptr)
			throw std::exception("Failed to initialize graphics");

		s_Device = std::make_shared<D3D11Device>(_device);
		s_RenderContext = std::make_shared<D3D11RenderContext>(_deviceContext);
		s_SwapChain = std::make_shared<D3D11SwapChain>(_swapChain);
	
		ID3D11Device* _pDevice = reinterpret_cast<ID3D11Device*>(s_Device->GetNativePointer());
		
		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		WRECK_HR(_pDevice->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice));

		Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
		WRECK_HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), &dxgiAdapter));
		
		DXGI_ADAPTER_DESC adapterDesc = {};

		dxgiAdapter->GetDesc(&adapterDesc);

		s_Capabilities.GPU_Name = Misc::StringHelper::ToNarrow(adapterDesc.Description);
		s_Capabilities.Memory_Size = adapterDesc.DedicatedVideoMemory / 1024 / 1024;
	}

	void Renderer::InitializeOpenGL(IWindow& window)
	{
		WRECK_ASSERT(false, "Not yet implemented");
	}


	// Graphics exception stuff
	Renderer::HrException::HrException(const char* file, int line, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
		:
		Exception(file, line),
		m_HR(hr)
	{
		// join all info messages with newlines into single string
		for (const auto& m : infoMsgs)
		{
			m_Info += m;
			m_Info.push_back('\n');
		}
		// remove final newline if exists
		if (!m_Info.empty())
		{
			m_Info.pop_back();
		}
	}

	const char* Renderer::HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
			<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
			<< "[Error String] " << GetErrorString() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl;
		if (!m_Info.empty())
		{
			oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
		}
		oss << GetOriginalString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	const char* Renderer::HrException::GetType() const noexcept
	{
		return "Chili Graphics Exception";
	}

	HRESULT Renderer::HrException::GetErrorCode() const noexcept
	{
		return m_HR;
	}

	std::string Renderer::HrException::GetErrorString() const noexcept
	{
		return DXGetErrorString(m_HR);
	}

	std::string Renderer::HrException::GetErrorDescription() const noexcept
	{
		char buf[512];
		DXGetErrorDescription(m_HR, buf, sizeof(buf));
		return buf;
	}

	std::string Renderer::HrException::GetErrorInfo() const noexcept
	{
		return m_Info;
	}

	const char* Renderer::DeviceRemovedException::GetType() const noexcept
	{
		return "Chili Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
	}
	Renderer::InfoException::InfoException(const char* file, int line, std::vector<std::string> infoMsgs) noexcept
		:
		Exception(file, line )
	{
		// join all info messages with newlines into single string
		for (const auto& m : infoMsgs)
		{
			m_Info += m;
			m_Info.push_back('\n');
		}
		// remove final newline if exists
		if (!m_Info.empty())
		{
			m_Info.pop_back();
		}
	}


	const char* Renderer::InfoException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
		oss << GetOriginalString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	const char* Renderer::InfoException::GetType() const noexcept
	{
		return "Chili Graphics Info Exception";
	}

	std::string Renderer::InfoException::GetErrorInfo() const noexcept
	{
		return m_Info;
	}
}