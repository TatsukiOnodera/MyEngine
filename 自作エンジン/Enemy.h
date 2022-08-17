#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Object3d.h"

class Enemy
{
public: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // メンバ変数
	// オブジェクト
	std::unique_ptr<Object3d> m_object = nullptr;
	// 移動ベクトル
	XMFLOAT3 m_vec = { 0, 0, 0 };
	// 生存フラグ
	bool m_alive = false;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy(Object3d* enemy);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(XMFLOAT3 playerPosition);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 始点から終点への距離
	/// </summary>
	/// <param name="pos1">終点</param>
	/// <param name="pos2">始点</param>
	/// <returns>二点間の距離</returns>
	const float Length(XMFLOAT3 pos1, XMFLOAT3 pos2);

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	XMFLOAT3 GetPosition() { return m_object->GetPosition(); }
};
