#pragma once
#include <DirectXMath.h>

class Object3d;
class BaseCollider;

struct CollisionInfo
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="object">衝突相手のオブジェクト</param>
	/// <param name="collider">衝突相手のコライダー</param>
	/// <param name="inter">衝突点</param>
	CollisionInfo(Object3d* object, BaseCollider* collider, const DirectX::HXMVECTOR& inter)
	{
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}

public: //メンバ変数
	//衝突相手のオブジェクト
	Object3d* object = nullptr;
	//衝突相手のコライダー
	BaseCollider* collider = nullptr;
	//衝突店
	DirectX::XMVECTOR inter;
};