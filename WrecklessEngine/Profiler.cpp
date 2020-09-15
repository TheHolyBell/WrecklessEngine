#include "Profiler.h"

namespace Profiling
{
	void Profiler::BeginSession(const std::string& name, const std::string& filepath)
	{
		std::lock_guard _LockGuard(m_Mutex);
		if (m_CurrentSession != nullptr)
		{
			// If there's already a curent session, then close it efore beginning new one.
			// Subsequent profiling output meant for the origianl session will end up in the 
			// newly opened session instead. That's better than having badly formatted
			// profiling output
			WRECK_ASSERT(false, "BeginSession already open in another thread");
			InternalEndSession();
		}
		m_OutputStream.open(filepath);

		if (m_OutputStream.is_open())
		{
			m_CurrentSession = new ProfileSession{ name };
			WriteHeader();
		}
		else
		{
			WRECK_ASSERT(false, "Instrumentor couldn't open results file");
		}
	}
	void Profiler::EndSession()
	{
		std::lock_guard _LockGuard(m_Mutex);
		InternalEndSession();
	}
	void Profiler::WriteProfile(const ProfileResult& result)
	{
		std::ostringstream json;

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
		json << "\"name\":\"" << result.Name << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << result.ThreadID << ",";
		json << "\"ts\":" << result.Start.count();
		json << "}";

		std::lock_guard _LockGuard(m_Mutex);
		if (m_CurrentSession != nullptr)
		{
			m_OutputStream << json.str();
			m_OutputStream.flush();
		}
	}
	Profiler& Profiler::GetInstance()
    {
        static Profiler _Profiler;
        return _Profiler;
    }
    Profiler::~Profiler()
    {
        EndSession();
    }
	void Profiler::WriteHeader()
	{
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
		m_OutputStream.flush();
	}
	void Profiler::WriteFooter()
	{
		m_OutputStream << "]}";
		m_OutputStream.flush();
	}

	// Note: you must already own lock on m_Mutex before
	// calling InternalEndSession

	void Profiler::InternalEndSession()
	{
		if (m_CurrentSession != nullptr)
		{
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
		}
	}
}