#pragma once
#include <DirectXMath.h>

/// <summary>
/// 便利機能
/// </summary>
class Tool
{
public: // 静的メンバ関数
	/// <summary>
	/// XMFLOAT3同士の長さを計算
	/// </summary>
	/// <param name="tmp1">終点</param>
	/// <param name="tmp2">始点</param>
	/// <returns>長さ</returns>
	static float LengthFloat3(const DirectX::XMFLOAT3& tmp1, const DirectX::XMFLOAT3& tmp2 = { 0, 0, 0 });

	/// <summary>
	/// XMFLOAT3同士の長さを計算
	/// </summary>
	/// <param name="tmp1">終点</param>
	/// <param name="tmp2">始点</param>
	/// <returns>長さ</returns>
	static DirectX::XMFLOAT3 VectorFloat3(const DirectX::XMFLOAT3& tmp1, const DirectX::XMFLOAT3& tmp2 = { 0, 0, 0 });

	/// <summary>
	/// XMFLOAT3を正規化
	/// </summary>
	/// <param name="tmp">XMFLOAT3</param>
	/// <param name="num">正規化する値</param>
	/// <returns>変換されたXMFLOAT3</returns>
	static DirectX::XMFLOAT3 NormalizeFloat3(const DirectX::XMFLOAT3& tmp, float num = 1.0f);
};
