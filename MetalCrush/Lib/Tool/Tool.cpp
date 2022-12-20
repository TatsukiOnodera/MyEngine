#include "Tool.h"

float Tool::LengthFloat3(const DirectX::XMFLOAT3& tmp1, const DirectX::XMFLOAT3& tmp2)
{
	DirectX::XMFLOAT3 len = { tmp1.x - tmp2.x, tmp1.y - tmp2.y, tmp1.z - tmp2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}

DirectX::XMFLOAT3 Tool::VectorFloat3(const DirectX::XMFLOAT3& tmp1, const DirectX::XMFLOAT3& tmp2)
{
	DirectX::XMFLOAT3 len = { tmp1.x - tmp2.x, tmp1.y - tmp2.y, tmp1.z - tmp2.z };

	return len;
}

DirectX::XMFLOAT3 Tool::NormalizeFloat3(const DirectX::XMFLOAT3& tmp, float num)
{
	DirectX::XMFLOAT3 _normalize = tmp;

	float len = LengthFloat3(tmp);
	if (len > num)
	{
		_normalize.x /= len;
		_normalize.y /= len;
		_normalize.z /= len;
	}

	_normalize.x *= num;
	_normalize.y *= num;
	_normalize.z *= num;

	return _normalize;
}
