#include "Application.h"
#include "ImGui/imgui.h"
#include "Renderer.h"
#include "GlobalClock.h"
#include "ImGuiLogger.h"
#include "Manipulators.h"
#include "FileDialog.h"
#include "SceneCamera.h"

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

		m_pTexture = Graphics::Renderer::GetDevice()->CreateTexture2D("D:\\Downloads\\image.png");
		m_Camera.SetFrustumProperties(3.14159 / 2, (float)width  / (float)height, 0.1f, 100);

		m_Camera.LookAt(DirectX::XMFLOAT3{ 10.0f, 5.0f, -20.0f }, DirectX::XMFLOAT3{0,0,0},
			DirectX::XMFLOAT3{ 0,1,0 });
		m_Camera.UpdateViewMatrix();
		CameraSystem::SceneCamera::SetView(m_Camera.GetView());
		CameraSystem::SceneCamera::SetProjection(m_Camera.GetProjection());

		m_Viewport.Width = width;
		m_Viewport.Height = height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		Graphics::Renderer::GetRenderContext()->BindViewport(m_Viewport);

		m_pCube = std::make_shared<Misc::TestCube>(5);
	}
	void Application::Run()
	{
		OnInit();

		while (m_Running)
		{
			if (!m_Minimized)
			{
				float color[] = { 0.2f, 0.6, 0.8, 1.0f };
				auto backBuffer = Graphics::Renderer::GetSwapChain()->GetBackBuffer();
				auto depthBuffer = Graphics::Renderer::GetSwapChain()->GetDepthStencilView();

				Graphics::Renderer::GetRenderContext()->ClearRenderTarget(backBuffer, color);
				Graphics::Renderer::GetRenderContext()->ClearDepthStencilView(depthBuffer, 1.0f);
				Graphics::Renderer::GetRenderContext()->SetOutputTargets(backBuffer, depthBuffer);
				Profiling::GlobalClock::Update();
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate();

				m_pCube->Draw();

				RenderImGui();
			}

			m_pWindow->OnUpdate();
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
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
					IO::cout << *FileSystem::FileDialog::OpenFile() << IO::endl;
				if (ImGui::MenuItem("Save"))
					IO::cout << *FileSystem::FileDialog::SaveFile() << IO::endl;
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Data"))
			{
				if (ImGui::MenuItem("Clear World"))
					IO::cout << "Clear World command has been requested" << IO::endl;
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		using namespace Graphics;
		if (ImGui::Begin("Renderer"))
		{
			auto& caps = Renderer::GetCapabilities();
			ImGui::Text("GPU: %s", caps.GPU_Name.c_str());
			ImGui::Text("Memory size: %d MB", caps.Memory_Size);
			ImGui::Text("Delta Time: %.5fs", Profiling::GlobalClock::DeltaTime());
			ImGui::Text("Total Time: %.2fs", Profiling::GlobalClock::TotalTime());
			ImGui::Text("Framerate: %d", Profiling::GlobalClock::GetFrameCount());
			ImGui::Text("Image dimensions: %dx%d", m_pTexture->GetWidth(), m_pTexture->GetHeight());
			ImGui::Image((ImTextureID)m_pTexture->GetNativePointer(), ImVec2(m_pTexture->GetWidth(), m_pTexture->GetHeight()));
		}
		ImGui::End();
		IO::ImGuiOutput::Draw();

		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();

		m_ImGuiLayer->End();
	}
	Graphics::IWindow& Application::GetWindow()
	{
		return *m_pWindow;
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
		Graphics::Renderer::GetRenderContext()->BindViewport(vp);
		Graphics::Renderer::GetSwapChain()->ResizeBuffers(width, height);
	
		return false;
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
