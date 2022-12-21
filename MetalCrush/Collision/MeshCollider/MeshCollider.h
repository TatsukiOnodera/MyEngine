#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// メッシュ衝突判定
/// </summary>
class MeshCollider : public BaseCollider
{
private: // 定数
	static const int c_maxAreaX = 16;
	static const int c_maxAreaY = c_maxAreaX;

private: // サブクラス
	// 三角形の情報
	struct AreaInfo
	{
		// 三角形の情報
		std::vector<Triangle> triangle;

		// デストラクタ
		~AreaInfo()
		{
			triangle.clear();
		}
	};

private: // メンバ変数
	// 三角形の配列
	AreaInfo m_area[c_maxAreaY][c_maxAreaX];
	// ワールド行列の逆行列
	DirectX::XMMATRIX m_invMatWorld = {};
	// 正の最大座標
	DirectX::XMFLOAT3 positivePosition = {};
	// 負の最大座標
	DirectX::XMFLOAT3 negativePosition = {};

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MeshCollider();

	/// <summary>
	/// 三角形の配列を作る
	/// </summary>
	/// <param name="model">モデルデータ</param>
	/// <param name="div">縦横に何分割するか</param>
	void ConstructTriangles(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 三角形をエリア分け
	/// </summary>
	/// <param name="tri">三角形の情報</param>
	/// <returns>エリア番号</returns>
	void SortOutTriangleArea(const Triangle& tri);

	/// <summary>
	/// 座標がどのエリアにいるか
	/// </summary>
	/// <param name="pos">座標</param>
	/// <returns>該当のエリア</returns>
	std::vector<Triangle> GetPositionAreaNumber(const DirectX::XMVECTOR& pos);

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
