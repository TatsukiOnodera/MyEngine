#include "Tool.h"

float Tool::Length3to3(const DirectX::XMFLOAT3& tmp1, const DirectX::XMFLOAT3& tmp2)
{
	DirectX::XMFLOAT3 len = { tmp1.x - tmp2.x, tmp1.y - tmp2.y, tmp1.z - tmp2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}

DirectX::XMFLOAT3 Tool::Normalize3(const DirectX::XMFLOAT3& tmp)
{
	DirectX::XMVECTOR _normalize = { tmp.x, tmp.y, tmp.z, 0 };

	DirectX::XMVector3Normalize(_normalize);

	return DirectX::XMFLOAT3(_normalize.m128_f32[0], _normalize.m128_f32[1], _normalize.m128_f32[2]);
}
