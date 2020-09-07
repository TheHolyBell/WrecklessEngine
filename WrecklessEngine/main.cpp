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


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int)
{
	using namespace Input;
	using namespace IO;
	Console::Initialize();

	/*std::hash<std::string> hasher;
	
	IO::cout << hasher("dick") << IO::endl;

	std::hash<int> hasher2;
	std::hash<int> hasher3;

	IO::cout << hasher2(1488) << IO::endl;
	IO::cout << hasher3(1488) << IO::endl;*/

	/*IO::cout << HASH(1488) << IO::endl;
	IO::cout << HASH(1488) << IO::endl;
	IO::cout << HASH("Dick Gibson") << IO::endl;

	int value;
	IO::cin >> value;

	IO::cout << HASH(value) << IO::endl;*/

	/*std::string joe = "Joe Huy Na";
	const char* john = "Joe Huy Na";

	IO::cout << HASH(joe) << IO::endl;
	IO::cout << HASH(john) << IO::endl;*/

	
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
	Scripting::KeyboardCSharp::Bind();
	Scripting::TimeCSharp::Bind();
	Scripting::NoiseCSharp::Bind();
	Scripting::ComponentsCSharp::Bind();
	Scripting::EntityCSharp::Bind();


	auto klass = domain.GetClass("Sandbox", "Sandbox");

	std::shared_ptr<ECS::Scene> scene = std::make_shared<ECS::Scene>("main");
	ECS::Entity ent = scene->CreateEntity();
	ECS::Entity ent2 = scene->CreateEntity();
	ECS::Entity ent3 = scene->CreateEntity();
	ECS::Entity ent4 = scene->CreateEntity();
	ECS::Entity ent5 = scene->CreateEntity();
	ECS::Entity ent6 = scene->CreateEntity();

	ECS::SceneManager::AddScene(scene);
	ECS::SceneManager::SetActiveScene("main");

	ent.AddComponent<ECS::ScriptComponent> (ent.GetID(),domain.GetClass("Sandbox", "Actor")).Object().GetProperty("Name").Set("Dick");
	ent2.AddComponent<ECS::ScriptComponent>(ent.GetID(),domain.GetClass("Sandbox", "Actor")).Object().GetProperty("Name").Set("Richard");
	ent3.AddComponent<ECS::ScriptComponent>(ent.GetID(),domain.GetClass("Sandbox", "Actor")).Object().GetProperty("Name").Set("Edward");
	ent4.AddComponent<ECS::ScriptComponent>(ent.GetID(),domain.GetClass("Sandbox", "Actor")).Object().GetProperty("Name").Set("Sally");
	ent5.AddComponent<ECS::ScriptComponent>(ent.GetID(),domain.GetClass("Sandbox", "Actor")).Object().GetProperty("Name").Set("Nathan");
	ent6.AddComponent<ECS::ScriptComponent>(ent.GetID(),domain.GetClass("Sandbox", "Shkura"));

	//ent.AddComponent<ECS::ScriptComponent>(ent.GetID(),domain.GetClass("Sandbox", "Shkura"));

	if (ent.HasComponent<ECS::TransformComponent>())
		IO::cout << "ent has TransformComponent" << IO::endl;

	try
	{
		klass.Invoke("Dickie");
	}
	catch (std::exception& exc)
	{
		IO::cout << exc.what() << IO::endl;
	}


	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		Keyboard::Update();
		//GamePad::Get().UpdateState();
		Profiling::GlobalClock::Update();
		WRECK_PROFILE_FUNCTION();
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//if (Keyboard::IsKeyDown(KeyCode::SPACE)) IO::cout << "Space is down!!" << IO::endl;
			//IO::cout << "Window dimensions: " << pWindow->GetWidth() << "-" << pWindow->GetHeight() << IO::endl;
			//IO::cout << Profiling::Profiler::GetInstance().GetStatistics() << IO::endl;
			//Profiling::Profiler::GetInstance().Reset();
			//Scripting::ParameterList params;
			//params.Append(Profiling::GlobalClock::GetDelta());
			//klass.Invoke("UpdateRoutine");
			scene->OnUpdate();
		}
	}

	//IO::cin.Get();

	return 0;
}