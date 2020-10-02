#include "Application.h"
#include "ImGui/imgui.h"
#include "Renderer.h"
#include "GlobalClock.h"
#include "ImGuiLogger.h"
#include "Manipulators.h"
#include "FileDialog.h"
#include "SceneCamera.h"
#include "Pipeline.h"
#include "SceneManager.h"
#include "GamePad.h"

#define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this, std::placeholders::_1)

static Wreckless::Application* g_AppInstance = nullptr;

namespace Wreckless
{
	Application::Application(const std::string& windowName, int width, int height)
	{

		WRECK_ASSERT(g_AppInstance == nullptr, "Application already has been created");
		g_AppInstance = this;

		m_pWindow = std::make_unique<Graphics::Win32Window>(windowName.c_str(), width, height);
		m_pWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
	
		Graphics::Renderer::Initialize(Graphics::RendereringAPI::DirectX11, *m_pWindow);

		m_ImGuiLayer = new ImGuiLayer("ImGui");
		PushOverlay(m_ImGuiLayer);
		
		m_Viewport.Width = width;
		m_Viewport.Height = height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		Graphics::Renderer::GetRenderContext()->BindViewport(m_Viewport);

		Graphics::Pipeline::Initialize(width, height);
	}
	void Application::Run()
	{
		OnInit();

		MSG msg = {};
		while (msg.message != WM_QUIT && m_Running)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				if (!m_Minimized)
				{
					Input::GamePad::Get().UpdateState();

					float color[] = { 0.2f, 0.6, 0.8, 1.0f };

					Graphics::Pipeline::Execute();

					auto backBuffer = Graphics::Renderer::GetSwapChain()->GetBackBuffer();
					auto depthBuffer = Graphics::Renderer::GetSwapChain()->GetDepthStencilView();

					Graphics::Renderer::GetRenderContext()->ClearRenderTarget(backBuffer, color);
					Graphics::Renderer::GetRenderContext()->ClearDepthStencilView(depthBuffer, 1.0f);
					Graphics::Renderer::GetRenderContext()->SetOutputTarget(backBuffer, depthBuffer);
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate();


					RenderImGui();
					m_pWindow->OnUpdate();
				}
			}
		}

		OnShutdown();
	}
	void Application::OnEvent(Event& event)
	{
		WRECK_PROFILE_FUNCTION();
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}
	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
	void Application::RenderImGui()
	{
		WRECK_PROFILE_FUNCTION();
		m_ImGuiLayer->Begin();
		
		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();

		m_ImGuiLayer->End();
	}
	Graphics::IWindow& Application::GetWindow()
	{
		return *m_pWindow;
	}
	void Application::Close()
	{
		m_Running = false;
	}
	Application& Application::GetInstance()
	{
		WRECK_ASSERT(g_AppInstance, "App hasn't been created");
		return *g_AppInstance;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		int width = e.GetWidth(), height = e.GetHeight();
		if (width == 0 || height == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;

		m_Viewport = {};
		m_Viewport.Width = width;
		m_Viewport.Height = height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		//Graphics::Renderer::GetRenderContext()->BindViewport(vp);
		Graphics::Renderer::GetSwapChain()->ResizeBuffers(width, height);
		Graphics::Pipeline::ResizeBuffers(width, height);
	
		for (Layer* layer : m_LayerStack)
			layer->OnResize(width, height);

		return false;
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
