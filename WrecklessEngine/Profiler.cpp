#include "Profiler.h"

namespace Profiling
{
    Profiler& Profiler::GetInstance()
    {
        static Profiler _Profiler;
        return _Profiler;
    }
    void Profiler::StoreSample(const std::string& name, float time)
    {
        m_OSS << "[" << time * 1000 << "ms] " << name << "\n";
    }
    void Profiler::Reset()
    {
        m_OSS.str("");
        m_OSS.clear();
    }
    std::string Profiler::GetStatistics()
    {
        return m_OSS.str();
    }
    Profiler::Profiler()
    {
        m_OSS << std::fixed;
    }
}