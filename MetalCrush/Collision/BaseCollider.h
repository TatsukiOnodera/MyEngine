#pragma once
#include "CollisionType.h"
#include "Object3d.h"

class BaseCollider
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BaseCollider() = default;

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~BaseCollider() = default;

	/// <summary>
	/// オブジェクトをセット
	/// </summary>
	/// <param name="object">セットするオブジェクト</param>
	inline void SetObject(Object3d* object) { this->object3d = object; }

	/// <summary>
	/// オブジェクトを取得
	/// </summary>
	/// <returns>格納したオブジェクト</returns>
	inline Object3d* GetObject3d() { return object3d; }

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 形状タイプ取得
	/// </summary>
	/// <returns>形状タイプ</returns>
	inline CollisionShapeType GetShapeType() { return shapeType; }


	inline void OnCollision(const CollisionInfo& info) { object3d->OnCollision(info); }

protected: //メンバ変数
	//オブジェクト格納先
	Object3d* object3d = nullptr;
	//形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};