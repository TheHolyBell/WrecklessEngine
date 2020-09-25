#include "GlobalClock.h"
#include "InputOutputStream.h"

namespace Profiling
{
	GlobalClock::GlobalClock()
	{
		m_CurrentFrameTime = m_LastFrameTime = m_StartTime = std::chrono::high_resolution_clock::now();
	}
	float GlobalClock::DeltaTime()
	{
		return GetInstance().m_Delta;
	}
	float GlobalClock::TotalTime()
	{
		return std::chrono::duration<float>(GetInstance().m_CurrentFrameTime - GetInstance().m_StartTime).count();
	}
	void GlobalClock::Reset()
	{
		auto& _Instance = GetInstance();
		_Instance.m_CurrentFrameTime = _Instance.m_LastFrameTime = _Instance.m_StartTime = std::chrono::high_resolution_clock::now();
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
		_Instance.m_CurrentFrameTime = std::chrono::high_resolution_clock::now();
		_Instance.m_Delta = std::chrono::duration<float>(_Instance.m_CurrentFrameTime - _Instance.m_LastFrameTime).count();
		_Instance.m_LastFrameTime = _Instance.m_CurrentFrameTime;
		_Instance.m_FrameCount = (int)(1.0 / GetInstance().m_Delta);
	}
}
