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
#include "KeyboardCSharp.h"
#include "DebugConsoleCSharp.h"
#include "TimeCSharp.h"
#include "EntityCSharp.h"
#include "NoiseCSharp.h"
#include "ComponentsCSharp.h"

#include "GlobalClock.h"
#include "Timer.h"

#include "Hasher.h"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "SceneManager.h"

#include "Renderer.h"

#include "FileHelper.h"
#include "Application.h"
#include "FinalApplication.h"

#include <crtdbg.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int)
{
	using namespace Input;
	using namespace IO;
	Console::Initialize();
	

#if defined(DEBUG) | defined(_DEBUG)
_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Profiling::Profiler::GetInstance().BeginSession("Wreckless");

	try
	{
		Wreckless::Application* app = new Wreckless::FinalApplication("Wreckless Engine", 1600, 900);
		app->Run();
		delete app;
	}
	catch (std::exception& exc)
	{
		IO::cout << exc.what() << IO::endl;
		IO::cin.Get();
		return 0;
	}
	Profiling::Profiler::GetInstance().EndSession();
	return 0;
}