#pragma once
#include "BaseCollider.h"
#include <DirectXMath.h>

class Object3d;

/// <summary>
/// レイキャストの情報
/// </summary>
struct RayCast
{
	// 衝突相手のオブジェクト
	Object3d* m_object = nullptr;
	// 衝突相手のコライダー
	BaseCollider* m_collider = nullptr;
	// 衝突点
	DirectX::XMVECTOR m_inter;
	// 衝突点までの距離
	float m_distance = 0.0f;
};
