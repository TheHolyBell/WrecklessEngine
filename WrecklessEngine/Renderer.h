#pragma once
#include "IDevice.h"
#include "IRenderContext.h"
#include "InfoManager.h"
#include "WrecklessException.h"
#include "IWindow.h"
#include <memory>

namespace Graphics
{
	enum class RendereringAPI
	{
		DirectX11,
		OpenGL
	};

	class Renderer
	{
	public:

#pragma region Exceptions
		class Exception : public Exceptions::WrecklessException
		{
			using Exceptions::WrecklessException::WrecklessException;
		};
		class HrException : public Exception
		{
		public:
			HrException(const char* file, int line, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
			virtual const char* what() const noexcept override;
			virtual const char* GetType() const noexcept override;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorString() const noexcept;
			std::string GetErrorDescription() const noexcept;
			std::string GetErrorInfo() const noexcept;
		private:
			HRESULT m_HR;
			std::string m_Info;
		};
		class InfoException : public Exception
		{
		public:
			InfoException(const char* file, int line, std::vector<std::string> infoMsgs) noexcept;
			virtual const char* what() const noexcept override;
			virtual const char* GetType() const noexcept override;
			std::string GetErrorInfo() const noexcept;
		private:
			std::string m_Info;
		};
		class DeviceRemovedException : public HrException
		{
			using HrException::HrException;
		public:
			virtual const char* GetType() const noexcept override;
		private:
			std::string m_Reason;
		};
#pragma endregion

	public:
		Renderer(RendereringAPI apiType, std::shared_ptr<IWindow> window);

		Renderer(const Renderer& rhs) = delete;
		Renderer& operator=(const Renderer& rhs) = delete;
		
		Renderer(Renderer&& rhs) = delete;
		Renderer& operator=(Renderer&& rhs) = delete;



		std::shared_ptr<IDevice> GetDevice();
		std::shared_ptr<IRenderContext> GetRenderContext();
		std::shared_ptr<InfoManager> GetInfoManager();
	private:
		std::shared_ptr<IDevice> m_Device;
		std::shared_ptr<IRenderContext> m_RenderContext;
		std::shared_ptr<InfoManager> m_InfoManager;
	};
}