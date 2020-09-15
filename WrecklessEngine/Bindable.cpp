#include "Bindable.h"
#include "Renderer.h"

namespace Bindable
{
    Ref<Graphics::InfoManager> IBindable::GetInfoManager()
    {
        return Graphics::Renderer::GetInfoManager();
    }

}