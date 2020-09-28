#pragma once
#include "Application.h"
#include "EditorLayer.h"

namespace Wreckless
{
	class FinalApplication : public Application
	{
	public:
		FinalApplication(const std::string& windowName, int width, int height);
		virtual void OnInit() override;
	private:
		int m_Width = 0;
		int m_Height = 0;
	};
}