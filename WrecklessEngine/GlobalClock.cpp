#include "GlobalClock.h"

namespace Profiling
{
	GlobalClock::GlobalClock()
	{
		m_LastFrameTime = m_StartTime = std::chrono::high_resolution_clock::now();
	}
	float GlobalClock::GetDelta() const
	{
		return m_Delta;
	}
	float GlobalClock::TotalTime() const
	{
		auto _CurrTime = std::chrono::high_resolution_clock::now();
		return std::chrono::duration<float>(_CurrTime - m_StartTime).count();
	}
	void GlobalClock::Update()
	{
		auto _CurrTime = std::chrono::high_resolution_clock::now();
		m_Delta = std::chrono::duration<float>(_CurrTime - m_LastFrameTime).count();
		m_LastFrameTime = _CurrTime;
	}
}
