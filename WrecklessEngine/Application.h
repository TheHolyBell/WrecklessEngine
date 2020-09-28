#pragma once

#include "CommonInclude.h"
#include "Event.h"
#include "Layer.h"
#include "LayerStack.h"
#include "ApplicationEvent.h"
#include "ImGuiLayer.h"
#include "Win32Window.h"
#include "ITexture.h"

#include "TestCube.h"
#include "PerspectiveCamera.h"
#include "Scene.h"
#include "ScriptDomain.h"

namespace Wreckless
{
	class Application
	{
	public:
		Application(const std::string& windowName, int width, int height);
		virtual ~Application() = default;

		void Run();

		virtual void OnInit() {};
		virtual void OnShutdown() {};
		virtual void OnUpdate() {};

		virtual void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void RenderImGui();

		Graphics::IWindow& GetWindow();

		void Close();
		static Application& GetInstance();
	private:
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		std::unique_ptr<Graphics::IWindow> m_pWindow;
		bool m_Running = true, m_Minimized = false;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		Graphics::Viewport m_Viewport = {};
	};
}