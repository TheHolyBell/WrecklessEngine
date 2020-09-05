#pragma once

namespace Scripting
{
	class IParameter
	{
	public:
		IParameter() = default;

		IParameter(const IParameter& rhs) = default;
		IParameter& operator=(const IParameter& rhs) = default;

		IParameter(IParameter&& rhs) = default;
		IParameter& operator=(IParameter&& rhs) = default;

		virtual void* ptr() noexcept = 0;
		virtual ~IParameter() = default;
	};
}