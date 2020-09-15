#pragma once
#include "CommonInclude.h"
#include "Bindable.h"
#include <type_traits>
#include <memory>
#include <unordered_map>

namespace Bindable
{
	class IDevice;
	class IRenderContext;

	class Codex
	{
	public:
		template<typename T, typename...Params>
		static Ref<T> Resolve(Params&&...p) noxnd
		{
			static_assert(std::is_base_of<IBindable, T>::value, "Can only resolve classes derived from Bindable");
			return Get().Resolve_<T>(std::forward<Params>(p)...);
		}
	private:
		template<typename T, typename...Params>
		Ref<T> Resolve_(Params&&...p) noxnd
		{
			const auto key = T::GenerateUID(std::forward<Params>(p)...);
			const auto i = m_Binds.find(key);
			if (i == m_Binds.end())
			{
				auto _pBindable = std::make_shared<T>(std::forward<Params>(p)...);
				m_Binds[key] = _pBindable;
				return _pBindable;
			}
			else
				return std::static_pointer_cast<T>(i->second);
		}
		static Codex& Get()
		{
			static Codex _Instance;
			return _Instance;
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<IBindable>> m_Binds;
	};
}