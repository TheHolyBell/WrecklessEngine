#pragma once

#include <functional>

#define HASH(val) Hashing::Hasher::HashValue(val)

namespace Hashing
{
	class Hasher
	{
	public:
		template<typename T>
		constexpr const static size_t HashValue(T value)
		{
			static std::hash<T> _Hasher;
			return _Hasher(value);
		}

		template<>
		constexpr const static size_t HashValue(const char* value)
		{
			return HashValue<std::string>(value);
		}
	};
}