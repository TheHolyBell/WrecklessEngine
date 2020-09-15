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

namespace Misc
{
	TestCube::TestCube(float size)
	{
		using namespace Bindable;

		auto model = Cube::MakeIndependentTextured();
		model.Transform(DirectX::XMMatrixScaling(size, size, size));
		model.SetNormalsIndependentFlat();

		const auto geometryTag = "$cube." + std::to_string(size);
		AddBind(VertexBuffer::Resolve(geometryTag, model.vertices));
		AddBind(IndexBuffer::Resolve(geometryTag, model.indices));

		auto pvs = VertexShader::Resolve("DickVS.cso");


		ID3D10Blob* bc = reinterpret_cast<ID3D10Blob*>(pvs->GetByteCode());
		AddBind(InputLayout::Resolve(ShaderReflector::GetLayoutFromShader(bc), bc));
		AddBind(std::move(pvs));

		AddBind(PixelShader::Resolve("DickPS.cso"));

		AddBind(Topology::Resolve());
		AddBind(std::make_shared<TransformCBuf>());
	}
	void TestCube::SetPos(DirectX::XMFLOAT3 pos) noexcept
	{
	}
	void TestCube::SetRotation(float roll, float pitch, float yaw) noexcept
	{
	}
	DirectX::XMMATRIX TestCube::GetTransformXM() const noexcept
	{
		return DirectX::XMMatrixIdentity();
	}
}