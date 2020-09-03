#include "Bindable.h"
#include "Renderer.h"

namespace Bindable
{

    std::shared_ptr<Graphics::InfoManager> IBindable::GetInfoManager(Graphics::Renderer& renderer)
    {
        return renderer.GetInfoManager();
    }

}