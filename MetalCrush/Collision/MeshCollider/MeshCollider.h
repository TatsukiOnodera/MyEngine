#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// メッシュ衝突判定
/// </summary>
class MeshCollider : public BaseCollider
{
private: // メンバ変数
	// 三角形の配列
	std::vector<Triangle> m_triangles;
	// ワールド行列の逆行列
	DirectX::XMMATRIX m_invMatWorld = {};

public: // メンバ関数
	MeshCollider()
	{
		m_shapeType = COLLISONSHAPE_MESH;
	}

	/// <summary>
	/// 三角形の配列を作る
	/// </summary>
	/// <param name="model"></param>
	void ConstructTriangles(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 球との当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="inter">交点</param>
	/// <returns>成否</returns>
	bool CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// レイとの当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="distance">距離</param>
	/// <param name="inter">交点</param>
	/// <returns>成否</returns>
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, DirectX::XMVECTOR *inter = nullptr);
};
