#include "Renderer.h"
#include "dxerr.h"
#include <sstream>
#include "DXGIInfoManager.h"

namespace Graphics
{
	Renderer::Renderer(RendereringAPI apiType, std::shared_ptr<IWindow> window)
	{
		m_InfoManager = std::make_shared<DXGIInfoManager>();
	}
	std::shared_ptr<IDevice> Renderer::GetDevice()
	{
		return m_Device;
	}
	std::shared_ptr<IRenderContext> Renderer::GetRenderContext()
	{
		return m_RenderContext;
	}
	std::shared_ptr<InfoManager> Renderer::GetInfoManager()
	{
		return m_InfoManager;
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