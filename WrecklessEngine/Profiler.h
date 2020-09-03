#pragma once
#include <string>
#include <sstream>

namespace Profiling
{
	class Profiler
	{
	public:
		Profiler(const Profiler& rhs) = delete;
		Profiler& operator=(const Profiler& rhs) = delete;
		
		static Profiler& GetInstance();
		void StoreSample(const std::string& name, float time);
		void Reset();
		std::string GetStatistics();
	private:
		Profiler();

		std::ostringstream m_OSS;
	};
}