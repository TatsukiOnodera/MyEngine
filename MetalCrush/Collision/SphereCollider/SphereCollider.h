#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

class SphereCollider : public BaseCollider, public Sphere
{
private: // エイリアス
	using XMVECTOR = DirectX::XMVECTOR;

private: // メンバ変数
	// 中心からのオフセット
	XMVECTOR m_offset;
	// 半径
	float m_radius;

public: // メンバ関数
	SphereCollider(XMVECTOR offset = { 0, 0, 0, 0 }, float radius = 1.0f)
	{
		m_offset = offset;
		m_radius = radius;

		// 球状
		m_shapeType = COLLISONSHAPE_SPHERE;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: // アクセッサ
	/// <summary>
	/// 半径を取得
	/// </summary>
	/// <returns></returns>
	inline float GetRadius() { return m_radius; }

	/// <summary>
	/// 半径をセット
	/// </summary>
	/// <param name="radius">半径</param>
	inline void SetRadius(float radius)
	{ 
		m_radius = radius;
	}

	/// <summary>
	/// オフセットを取得
	/// </summary>
	/// <returns>オフセット</returns>
	inline const XMVECTOR& GetOffset() { return m_offset; }

	/// <summary>
	/// オフセットをセット
	/// </summary>
	/// <param name="offset">オフセット</param>
	inline void SetOffset(const XMVECTOR& offset)
	{
		m_offset = offset;
	}
};
