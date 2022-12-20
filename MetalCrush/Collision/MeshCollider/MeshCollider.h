#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// メッシュ衝突判定
/// </summary>
class MeshCollider : public BaseCollider
{
private: // サブクラス
	// 三角形の情報
	struct TriangleInfo
	{
		// 三角形の情報
		Triangle triangle;
		// エリアのナンバー
		unsigned long int areaNumber = 0;
	};

private: // メンバ変数
	// 三角形の配列
	std::vector<TriangleInfo> m_triangles;
	// ワールド行列の逆行列
	DirectX::XMMATRIX m_invMatWorld = {};
	// 正の最大座標
	DirectX::XMFLOAT3 positivePosition = {};
	// 負の最大座標
	DirectX::XMFLOAT3 negativePosition = {};
	// 縦横に何分割するか
	 int m_division = 0;

public: // メンバ関数
	MeshCollider()
	{
		m_shapeType = COLLISONSHAPE_MESH;
	}

	/// <summary>
	/// 三角形の配列を作る
	/// </summary>
	/// <param name="model">モデルデータ</param>
	/// <param name="div">縦横に何分割するか</param>
	void ConstructTriangles(Model* model, int div);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 三角形をエリア分け
	/// </summary>
	/// <param name="tri">三角形の情報</param>
	/// <returns>エリア番号</returns>
	int SetTriangleAreaNumber(const Triangle& tri);

	/// <summary>
	/// 座標をエリア分け
	/// </summary>
	/// <param name="pos">座標</param>
	/// <returns>エリア番号</returns>
	int SetPositionAreaNumber(const DirectX::XMVECTOR& pos);

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
