#pragma once
#include "CommonInclude.h"
#include <string>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <thread>
#include <sstream>

namespace Profiling
{
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct ProfileSession
	{
		std::string Name;
	};

	class Profiler
	{
	public:
		Profiler(const Profiler& rhs) = delete;
		Profiler& operator=(const Profiler& rhs) = delete;
		
		Profiler(Profiler&& rhs) = delete;
		Profiler& operator=(Profiler&& rhs) = delete;

		void BeginSession(const std::string& name, const std::string& filepath = "profile_results.json");

		void EndSession();

		void WriteProfile(const ProfileResult& result);

		static Profiler& GetInstance();

	private:
		Profiler() = default;

		~Profiler();

		void WriteHeader();

		void WriteFooter();

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession
		void InternalEndSession();

	private:
		std::mutex m_Mutex;
		ProfileSession* m_CurrentSession = nullptr;
		std::ofstream m_OutputStream;
	};
}