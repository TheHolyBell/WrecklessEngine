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
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_bStopped;
	public:
		ScopedTimer(const char* name);
		~ScopedTimer();
		void Stop();
	};
}