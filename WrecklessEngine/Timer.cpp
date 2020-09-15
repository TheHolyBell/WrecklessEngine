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
		: m_Name(name), m_bStopped(false)
	{
		m_StartTimepoint = std::chrono::steady_clock::now();
	}

	ScopedTimer::~ScopedTimer()
	{
		if (!m_bStopped)
			Stop();
	}

	void ScopedTimer::Stop()
	{
		auto endTimepoint = std::chrono::steady_clock::now();
		auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
		auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();
	
		Profiler::GetInstance().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });
	}

}