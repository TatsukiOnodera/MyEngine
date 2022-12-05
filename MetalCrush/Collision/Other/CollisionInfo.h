#pragma once
#include <DirectXMath.h>

class Object3d;
class BaseCollider;

/// <summary>
/// 衝突情報
/// </summary>
struct CollisionInfo
{
public: //メンバ変数
	//衝突相手のオブジェクト
	Object3d* m_object = nullptr;
	//衝突相手のコライダー
	BaseCollider* m_collider = nullptr;
	//衝突点
	DirectX::XMVECTOR m_inter;

public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="object">衝突相手のオブジェクト</param>
	/// <param name="collider">衝突相手のコライダー</param>
	/// <param name="inter">衝突点</param>
	CollisionInfo(Object3d* object, BaseCollider* collider, const DirectX::HXMVECTOR& inter)
	{
		m_object = object;
		m_collider = collider;
		m_inter = inter;
	}
};