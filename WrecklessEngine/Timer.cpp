#include "Timer.h"
#include "Profiler.h"

namespace Profiling
{
	Timer::Timer()
	{
		m_Start = m_End = std::chrono::high_resolution_clock::now();
	}

	void Timer::Begin()
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

	void Timer::End()
	{
		m_End = std::chrono::high_resolution_clock::now();
	}

	float Timer::DeltaTime() const
	{
		return std::chrono::duration<float>(m_End - m_Start).count();
	}


	ScopedTimer::ScopedTimer(const char* name)
		: m_Name(name)
	{
		m_Timer.Begin();
	}

	ScopedTimer::~ScopedTimer()
	{
		m_Timer.End();
		Profiler::GetInstance().StoreSample(m_Name, m_Timer.DeltaTime());
	}

}