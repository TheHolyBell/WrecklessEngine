#include "FinalApplication.h"

namespace Wreckless
{
	FinalApplication::FinalApplication(const std::string& windowName, int width, int height)
		: Application(windowName, width, height), m_Width(width), m_Height(height)
	{
	}
	void FinalApplication::OnInit()
	{
		EditorLayer* layer = new EditorLayer();
		layer->OnResize(m_Width, m_Height);
		PushLayer(layer);
	}
}