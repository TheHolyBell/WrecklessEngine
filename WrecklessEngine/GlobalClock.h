#pragma once
#include <chrono>

namespace Profiling
{
	class GlobalClock
	{
		GlobalClock();

		std::chrono::steady_clock::time_point m_StartTime;
		std::chrono::steady_clock::time_point m_LastFrameTime;
		float m_Delta;
	public:

		GlobalClock(const GlobalClock& rhs) = delete;
		GlobalClock& operator=(const GlobalClock& rhs) = delete;

		static GlobalClock& GetInstance()
		{
			static GlobalClock _Instance;
			return _Instance;
		}

		float GetDelta() const;
		float TotalTime() const;
		// @ Call once per frame
		void Update();
	};
}