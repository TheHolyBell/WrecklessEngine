#pragma once
#include "CommonInclude.h"
#include "IDevice.h"
#include "IRenderContext.h"
#include "ISwapChain.h"
#include "InfoManager.h"
#include "WrecklessException.h"
#include "IWindow.h"
#include <memory>

namespace Graphics
{
	enum class RendereringAPI
	{
		None,
		DirectX11,
		OpenGL
	};

	struct RendererAPICapabilities
	{
		std::string GPU_Name;
		unsigned long long Memory_Size;
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

		Renderer(const Renderer& rhs) = delete;
		Renderer& operator=(const Renderer& rhs) = delete;
		
		Renderer(Renderer&& rhs) = delete;
		Renderer& operator=(Renderer&& rhs) = delete;

		static RendererAPICapabilities& GetCapabilities();

		static void Initialize(RendereringAPI apiType, IWindow& window);
		static Ref<IDevice> GetDevice();
		static Ref<IRenderContext> GetRenderContext();
		static Ref<ISwapChain> GetSwapChain();
		static Ref<InfoManager> GetInfoManager();
	private:
		static void InitializeD3D11(IWindow& window);
		static void InitializeOpenGL(IWindow& window);
	private:
		static Ref<IDevice> s_Device;
		static Ref<IRenderContext> s_RenderContext;
		static Ref<InfoManager> s_InfoManager;
		static Ref<ISwapChain> s_SwapChain;
		static RendererAPICapabilities s_Capabilities;
	};
}