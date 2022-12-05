#pragma once
#include <DirectXMath.h>

/// <summary>
/// �֗��@�\
/// </summary>
class Tool
{
public: // �ÓI�����o�֐�
	/// <summary>
	/// XMFLOAT3���m�̒������v�Z
	/// </summary>
	/// <param name="tmp1">�I�_</param>
	/// <param name="tmp2">�n�_</param>
	/// <returns>����</returns>
	static float Length3to3(const DirectX::XMFLOAT3& tmp1, const DirectX::XMFLOAT3& tmp2 = { 0, 0, 0 });

	/// <summary>
	/// XMFLOAT3�𐳋K��
	/// </summary>
	/// <param name="tmp">XMFLOAT3</param>
	/// <returns>�ϊ����ꂽXMFLOAT3</returns>
	static DirectX::XMFLOAT3 Normalize3(const DirectX::XMFLOAT3& tmp);
};
