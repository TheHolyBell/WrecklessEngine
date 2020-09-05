#include <Windows.h>
#include "ConsoleClass.h"
#include "InputOutputStream.h"
#include "Manipulators.h"
#include "Win32Window.h"
#include <memory>
#include "Profiler.h"
#include "Timer.h"
#include "Keyboard.h"
#include "GamePad.h"

#include "ScriptingEngine.h"
#include "GamePadCSharp.h"
#include "MouseCSharp.h"
#include "DebugConsoleCSharp.h"
#include "GlobalClock.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int)
{
	using namespace Input;
	using namespace IO;
	Console::Initialize();

	std::shared_ptr<Graphics::IWindow> pWindow = std::make_shared<Graphics::Win32Window>("Hello buddy", 800, 600);

	Keyboard::Initialize((HWND)pWindow->GetWindowHandle());

	pWindow->SetTitle("Dickson");
	pWindow->SetIcon("D:\\VisualStudio\\C++\\Hardware3D\\chili.ico");


	Scripting::DirectoriesInfo info;
	info.etc_location = "D:\\VisualStudio\\C++\\WrecklessEngine\\Vendor\\Mono\\etc";
	info.lib_location = "D:\\VisualStudio\\C++\\WrecklessEngine\\Vendor\\Mono\\lib";
	
	Scripting::ScriptingEngine::Initialize(&info);
	Scripting::ScriptDomain domain = Scripting::ScriptingEngine::GetDomain("Sandbox.dll");

	Scripting::Debug::Bind();
	Scripting::GamePadCSharp::Bind();
	Scripting::MouseCSharp::Bind();

	auto klass = domain.GetClass("Sandbox", "Sandbox");

	klass.Invoke("Function");

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		Keyboard::Update();
		GamePad::Get().UpdateState();
		Profiling::GlobalClock::GetInstance().Update();
		WRECK_PROFILE_FUNCTION();
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//if (Keyboard::IsKeyDown(Keys::SPACE)) IO::cout << "Space is down!!" << IO::endl;
			//IO::cout << "Window dimensions: " << pWindow->GetWidth() << "-" << pWindow->GetHeight() << IO::endl;
			//IO::cout << Profiling::Profiler::GetInstance().GetStatistics() << IO::endl;
			//Profiling::Profiler::GetInstance().Reset();
			Scripting::ParameterList params;
			params.Append(Profiling::GlobalClock::GetInstance().GetDelta());
			klass.Invoke("UpdateRoutine", params);
		}
	}

	IO::cin.Get();

	return 0;
}