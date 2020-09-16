#include "Drawable.h"
#include "IndexBuffer.h"
#include "Bindable.h"

using namespace Bindable;

namespace Drawables
{
	void Drawable::Draw()
	{
		for (auto& b : m_Binds)
		{
			b->Bind();
		}
		unsigned indexCount = m_pIndexBuffer->GetCount();
		Graphics::Renderer::GetRenderContext()->DrawIndexed(indexCount, 0, 0);
	}

	void Drawable::AddBind(Ref<IBindable> bind) noxnd
	{
		// special case for index buffer
		if (typeid(*bind) == typeid(IndexBuffer))
		{
			assert("Binding multiple index buffers not allowed" && m_pIndexBuffer == nullptr);
			m_pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
		}
		m_Binds.push_back(std::move(bind));
	}
	void Drawables::Drawable::Update()
	{
	}
}

