#include "LayoutCodex.h"

namespace Dynamic
{
   
    CookedLayout LayoutCodex::Resolve(RawLayout&& layout) noxnd
    {
        auto _Signature = layout.GetSignature();
        auto& _Map = Get_().m_Map;
        const auto i = _Map.find(_Signature);
        // Identical layout already exists
        if (i != _Map.end())
        {
            // Input layout is expected to be cleared after Resolve
            // so just throw away the layout tree
            layout.ClearRoot();
            return { i->second };
        }

        // Otherwise add laout root element to map
        auto _Result = _Map.insert({ std::move(_Signature),layout.DeliverRoot() });
        // Return layout with additional reference to root
        return { _Result.first->second };
    }


    LayoutCodex& LayoutCodex::Get_() noexcept
    {
        static LayoutCodex _Codex;
        return _Codex;
    }
}