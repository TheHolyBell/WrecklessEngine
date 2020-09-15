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
	};
}