#pragma once
#include "CollisionType.h"
#include "Object3d.h"

class BaseCollider
{
protected: //メンバ変数
	//オブジェクト格納先
	Object3d* m_object3d = nullptr;
	//形状タイプ
	CollisionShapeType m_shapeType = SHAPE_UNKNOWN;

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
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const CollisionInfo& info) { m_object3d->OnCollision(info); }

public: // アクセッサ
	/// <summary>
	/// オブジェクトをセット
	/// </summary>
	/// <param name="object">セットするオブジェクト</param>
	inline void SetObject(Object3d* object) { m_object3d = object; }

	/// <summary>
	/// オブジェクトを取得
	/// </summary>
	/// <returns>格納したオブジェクト</returns>
	inline Object3d* GetObject3d() { return m_object3d; }

	/// <summary>
	/// 形状タイプ取得
	/// </summary>
	/// <returns>形状タイプ</returns>
	inline CollisionShapeType GetShapeType() { return m_shapeType; }
};