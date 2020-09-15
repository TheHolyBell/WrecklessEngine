#include "TimeCSharp.h"
#include "GlobalClock.h"
#include "ScriptingEngine.h"

namespace Scripting
{
	void TimeCSharp::Bind()
	{
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Time::get_DeltaTime", Profiling::GlobalClock::DeltaTime);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Time::get_TotalTime", Profiling::GlobalClock::TotalTime);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Time::get_FrameCount", Profiling::GlobalClock::GetFrameCount);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Time::get_TimeScale", Profiling::GlobalClock::GetTimeScale);
		ScriptingEngine::DirectBindCallback("WrecklessScript.Core.Time::set_TimeScale", Profiling::GlobalClock::SetTimeScale);
	}
}