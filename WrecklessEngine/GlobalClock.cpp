#include "GlobalClock.h"

namespace Profiling
{
	GlobalClock::GlobalClock()
	{
		m_LastFrameTime = m_StartTime = std::chrono::high_resolution_clock::now();
	}
	float GlobalClock::GetDelta()
	{
		return GetInstance().m_Delta;
	}
	float GlobalClock::TotalTime()
	{
		auto _CurrTime = std::chrono::high_resolution_clock::now();
		return std::chrono::duration<float>(_CurrTime - GetInstance().m_StartTime).count();
	}
	float GlobalClock::GetTimeScale()
	{
		return GetInstance().m_TimeScale;
	}
	void GlobalClock::SetTimeScale(float timeScale)
	{
		GetInstance().m_TimeScale = timeScale;
	}
	int GlobalClock::GetFrameCount()
	{
		return GetInstance().m_FrameCount;
	}
	void GlobalClock::Update()
	{
		auto& _Instance = GetInstance();
		auto _CurrTime = std::chrono::high_resolution_clock::now();
		_Instance.m_Delta = std::chrono::duration<float>(_CurrTime - _Instance.m_LastFrameTime).count();
		_Instance.m_LastFrameTime = _CurrTime;
		_Instance.m_FrameCount = (int)(1.0 / GetInstance().m_Delta);
	}
}
