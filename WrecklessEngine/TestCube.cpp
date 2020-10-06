#include "TestCube.h"
#include "Cube.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "IndexBuffer.h"
#include "Topology.h"
#include "ShaderReflector.h"
#include "InputLayout.h"
#include "TransformCBuf.h"
#include "DepthStencil.h"
#include "Rasterizer.h"

using namespace Misc;
namespace Drawables
{
	TestCube::TestCube(unsigned entID, float size)
	{
		using namespace Bindable;

		auto model = Cube::MakeIndependentTextured();
		model.Transform(DirectX::XMMatrixScaling(size, size, size));
		model.SetNormalsIndependentFlat();

		const auto geometryTag = "$cube." + std::to_string(size);
		AddBind(VertexBuffer::Resolve(geometryTag, model.vertices));
		AddBind(IndexBuffer::Resolve(geometryTag, model.indices));

		auto pvs = VertexShader::Resolve("DummyVS.cso");


		ID3D10Blob* bc = reinterpret_cast<ID3D10Blob*>(pvs->GetByteCode());
		AddBind(InputLayout::Resolve(ShaderReflector::GetLayoutFromShader(bc), bc));
		AddBind(std::move(pvs));

		AddBind(PixelShader::Resolve("DummyPS.cso"));

		AddBind(Rasterizer::Resolve(false));
		AddBind(DepthStencil::Resolve());

		AddBind(Topology::Resolve());
		AddBind(std::make_shared<TransformCBuf>(entID));
	}
}