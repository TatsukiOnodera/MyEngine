#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include <DirectXMath.h>

class SphereCollider : public BaseCollider, public Sphere
{
private: //エイリアス
	//DirectXを省略
	using XMVECTOR = DirectX::XMVECTOR;

private:
	//オブジェクトの中心からのオフセット
	XMVECTOR offset;
	//半径
	float radius;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="offset">オフセット</param>
	/// <param name="radius">半径</param>
	SphereCollider(XMVECTOR offset = { 0, 0, 0, 0 }, float radius = 1.0f) : offset(offset), radius(radius)
	{
		//球形状をセット
		shapeType = COLLISONSHAPE_SPHERE;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 半径のセット
	/// </summary>
	/// <param name="radius">半径</param>
	inline void SetRadius(float radius) { this->radius = radius; }
};
