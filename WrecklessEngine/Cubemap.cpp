#include "Cubemap.h"
#include "Cube.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "TransformCBuf.h"
#include "PixelShader.h"
#include "ShaderReflector.h"
#include "Topology.h"
#include "Texture3D.h"
#include "Rasterizer.h"
#include "Sampler.h"
#include "DepthStencil.h"

namespace Drawables
{
	Cubemap::Cubemap(unsigned entID, const std::string& path)
		: m_Path(path)
	{
		using namespace Bindable;

		auto model = Misc::Cube::MakeIndependent(Dynamic::VertexLayout{}.Append(Dynamic::VertexLayout::Position3D));

		auto geometryTag = "CubeMap." + path;
		AddBind(VertexBuffer::Resolve( geometryTag, model.vertices));
		AddBind(IndexBuffer::Resolve(geometryTag, model.indices));

		AddBind(Texture3D::Resolve(path));

		AddBind(Sampler::Resolve());

		auto pvs = VertexShader::Resolve("CubeMapVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddBind(std::move(pvs));

		AddBind(PixelShader::Resolve("CubeMapPS.cso"));

		AddBind(InputLayout::Resolve(model.vertices.GetLayout(), pvsbc));

		AddBind(Topology::Resolve(Graphics::PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		AddBind(Rasterizer::Resolve(true));
		AddBind(DepthStencil::Resolve(DepthStencil::Func::LEQUAL, false));
		

		AddBind(std::make_shared<TransformCBuf>(entID));
	}
	std::string Cubemap::GetPath() const
	{
		return m_Path;
	}
}