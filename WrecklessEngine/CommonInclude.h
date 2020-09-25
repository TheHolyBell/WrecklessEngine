#pragma once
#include "ConditionalNoexcept.h"

#include <type_traits>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <Windows.h>
#include <cassert>
#include <functional>
#include "HrException.h"

#include "Timer.h"

#define BIND_MEM_FN( fn ) std::bind(&fn, this);
#define STRINGIFY_( s ) #s
#define STRINGIFY( s ) STRINGIFY_( s )

//#define PROFILING

#ifdef PROFILING
	#define WRECK_PROFILE_SCOPE(name) Profiling::ScopedTimer _profiler##__LINE__(name)
	#define WRECK_PROFILE_FUNCTION() WRECK_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define WRECK_PROFILE_SCOPE(name)
	#define WRECK_PROFILE_FUNCTION()
#endif


#ifdef _DEBUG

	#define WRECK_DEBUGBREAK() __debugbreak()
	#define WRECK_ASSERT( x, msg ) assert( (x) && msg )
	
	#define WRECK_HR( x ) if( FAILED(x) ) throw Exceptions::HrException(__FILE__, __LINE__, (x) );

#else

	#define WRECK_DEBUGBREAK()
	#define WRECK_ASSERT( x, msg ) assert( (x) && msg )
	#define WRECK_HR( x ) if( FAILED(x) ) assert(false)

#endif

#define WRECK_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define BIT(x) (1 << x)

#define PURE = 0

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using RefUnique = std::unique_ptr<T>;