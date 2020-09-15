#include "FinalApplication.h"

namespace Wreckless
{
	FinalApplication::FinalApplication(const std::string& windowName, int width, int height)
		: Application(windowName, width, height)
	{
	}
	void FinalApplication::OnInit()
	{
		PushLayer(new EditorLayer());
	}
}