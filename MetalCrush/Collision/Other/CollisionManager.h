#pragma once
#include "CollisionPrimitive.h"
#include "RayCast.h"
#include <d3d12.h>
#include <memory>
#include <forward_list>

class BaseCollider;

/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager
{
public: // 静的メンバ関数
	static CollisionManager* GetInstance();

private: // メンバ変数
	std::forward_list<BaseCollider*> m_colliders;

private: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CollisionManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CollisionManager() = default;

	/// <summary>
	/// コピーインストラクタ
	/// </summary>
	/// <param name="">インストラクタ</param>
	CollisionManager(const CollisionManager&) = delete;

	/// <summary>
	/// コピー代入演算子
	/// </summary>
	/// <param name="">インストラクタ</param>
	/// <returns>衝突マネージャ</returns>
	CollisionManager& operator=(const CollisionManager&) = delete;

public: // メンバ関数
	/// <summary>
	/// コライダー追加
	/// </summary>
	/// <param name="collider">コライダー</param>
	inline void AddCollider(BaseCollider* collider)
	{
		m_colliders.emplace_front(collider);
	}

	/// <summary>
	/// コライダー削除
	/// </summary>
	/// <param name="collider">コライダー</param>
	inline void RemoveCollider(BaseCollider* collider)
	{
		m_colliders.remove(collider);
	}

	/// <summary>
	/// レイキャスト
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="hitInfo">衝突情報</param>
	/// <param name="maxDistance">最大距離</param>
	/// <returns>成否</returns>
	bool RayCastHit(const Ray& ray, RayCast* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

	/// <summary>
	/// すべての衝突チェック
	/// </summary>
	void CheckAllCollisions();
};
