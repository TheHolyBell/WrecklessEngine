#pragma once
#include <chrono>

namespace Profiling
{
	class GlobalClock
	{
		GlobalClock();

		std::chrono::steady_clock::time_point m_StartTime;
		std::chrono::steady_clock::time_point m_LastFrameTime;
		std::chrono::steady_clock::time_point m_CurrentFrameTime;
		float m_Delta = 0;
		float m_TimeScale = 1;
		int m_FrameCount = 0;

		static GlobalClock& GetInstance()
		{
			static GlobalClock _Instance;
			return _Instance;
		}
	public:

		GlobalClock(const GlobalClock& rhs) = delete;
		GlobalClock& operator=(const GlobalClock& rhs) = delete;

		GlobalClock(GlobalClock&& rhs) = delete;
		GlobalClock& operator=(GlobalClock&& rhs) = delete;

		static float DeltaTime();
		static float TotalTime();

		static void Reset();

		static float GetTimeScale();
		static void SetTimeScale(float timeScale);

		static int GetFrameCount();
		// @ Call once per frame
		static void Update();
	};
}