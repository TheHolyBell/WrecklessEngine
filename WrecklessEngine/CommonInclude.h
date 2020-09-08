#pragma once
#include "ConditionalNoexcept.h"

#include <type_traits>
#include <memory>
#include <string>
#include <vector>
#include <Windows.h>
#include <cassert>

#define BIND_MEM_FN( fn ) std::bind(&fn, this);
#define STRINGIFY_( s ) #s
#define STRINGIFY( s ) STRINGIFY_( s )

#define WRECK_DEBUGBREAK() __debugbreak()
#define WRECK_ASSERT( x, msg ) assert( (x) && msg )
#define WRECK_PROFILE_SCOPE(name) Profiling::ScopedTimer _profiler##name(name)
#define WRECK_PROFILE_FUNCTION() WRECK_PROFILE_SCOPE(__func__)
#define WRECK_HR( x ) if( FAILED(x) ) assert(false)

#define PURE = 0

template<typename T>
using Ref = std::shared_ptr<T>;