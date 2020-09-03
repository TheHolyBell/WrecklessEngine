#pragma once
#include <string>
#include <chrono>

namespace Profiling
{
	class Timer
	{
		std::chrono::steady_clock::time_point m_Start;
		std::chrono::steady_clock::time_point m_End;
	public:
		Timer();

		void Begin();
		void End();
		float DeltaTime() const;
	};

	class ScopedTimer
	{
		Timer m_Timer;
		const char* m_Name;
	public:
		ScopedTimer(const char* name);
		~ScopedTimer();
	};
}