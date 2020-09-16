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
#include "Entity.h"
#include "Components.h"
#include "ScriptingEngine.h"

#include "GamePadCSharp.h"
#include "MouseCSharp.h"
#include "KeyboardCSharp.h"
#include "DebugConsoleCSharp.h"
#include "TimeCSharp.h"
#include "EntityCSharp.h"
#include "NoiseCSharp.h"
#include "ComponentsCSharp.h"

#define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this, std::placeholders::_1)

static Wreckless::Application* g_AppInstance = nullptr;

namespace Wreckless
{
	Application::Application(const std::string& windowName, int width, int height)
		: m_Domain("Sandbox.dll")
	{

		WRECK_ASSERT(g_AppInstance == nullptr, "Application already has been created");
		g_AppInstance = this;

		m_pWindow = std::make_unique<Graphics::Win32Window>(windowName.c_str(), width, height);
		m_pWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
	
		Graphics::Renderer::Initialize(Graphics::RendereringAPI::DirectX11, *m_pWindow);

		Scripting::Debug::Bind();
		Scripting::GamePadCSharp::Bind();
		Scripting::MouseCSharp::Bind();
		Scripting::KeyboardCSharp::Bind();
		Scripting::TimeCSharp::Bind();
		Scripting::NoiseCSharp::Bind();
		Scripting::ComponentsCSharp::Bind();
		Scripting::EntityCSharp::Bind();

		m_ImGuiLayer = new ImGuiLayer("ImGui");
		PushOverlay(m_ImGuiLayer);

		m_pTexture = Graphics::Renderer::GetDevice()->CreateTexture2D("D:\\Downloads\\image.png");
		m_Camera.SetFrustumProperties(3.14159 / 2, (float)width  / (float)height, 0.1f, 100);

		m_Camera.LookAt(DirectX::XMFLOAT3{ 10.0f, 5.0f, -20.0f }, DirectX::XMFLOAT3{0,0,0},
			DirectX::XMFLOAT3{ 0,1,0 });
		m_Camera.UpdateViewMatrix();
		
		m_Viewport.Width = width;
		m_Viewport.Height = height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		Graphics::Renderer::GetRenderContext()->BindViewport(m_Viewport);

		m_pScene = std::make_shared<ECS::Scene>("main");
		ECS::SceneManager::AddScene(m_pScene);

		

		auto ent = m_pScene->CreateEntity();
		ent.AddComponent<ECS::ScriptComponent>(ent.GetID(), m_Domain.GetClass( "Sandbox", "Actor"));
		ent.AddComponent<ECS::TransformComponent>(DirectX::XMMatrixTranslation(5.0, 2.0f, 10.0f));
		ent.AddComponent<ECS::MeshComponent>(std::make_shared<Drawables::TestCube>(ent.GetID() ,10));

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
					CameraSystem::SceneCamera::SetView(m_Camera.GetView());
					CameraSystem::SceneCamera::SetProjection(m_Camera.GetProjection());
					Profiling::GlobalClock::Update();
					Input::GamePad::Get().UpdateState();
					ECS::SceneManager::GetActiveScene()->OnUpdate();

					float color[] = { 0.2f, 0.6, 0.8, 1.0f };

					Graphics::Pipeline::Execute();

					auto backBuffer = Graphics::Renderer::GetSwapChain()->GetBackBuffer();
					auto depthBuffer = Graphics::Renderer::GetSwapChain()->GetDepthStencilView();

					Graphics::Renderer::GetRenderContext()->ClearRenderTarget(backBuffer, color);
					Graphics::Renderer::GetRenderContext()->ClearDepthStencilView(depthBuffer, 1.0f);
					Graphics::Renderer::GetRenderContext()->SetOutputTargets(backBuffer, depthBuffer);
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

		Graphics::Viewport vp = {};
		vp.Width = width;
		vp.Height = height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		//Graphics::Renderer::GetRenderContext()->BindViewport(vp);
		Graphics::Renderer::GetSwapChain()->ResizeBuffers(width, height);
	
		Graphics::Pipeline::ResizeBuffers(width, height);
		m_Camera.SetFrustumProperties(3.1459 / 2, (float)width / (float)height, 0.1f, 100.0f);
		return false;
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
