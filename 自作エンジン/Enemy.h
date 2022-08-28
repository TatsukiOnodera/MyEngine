#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Object3d.h"
#include "Player.h"
#include "Bullet.h"

class Enemy
{
public: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	/// <summary>
	/// 生成
	/// </summary>
	/// <returns>Enemy</returns>
	static Enemy* Create();

private: // メンバ変数
	//Enemy
	// オブジェクト
	std::unique_ptr<Object3d> m_object = nullptr;
	// 座標
	XMFLOAT3 m_pos = { 0, 0, 50 };
	// 移動ベクトル
	XMFLOAT3 m_vec = { 0, 0, 0 };
	// 生存フラグ
	bool m_alive = false;
	//ショットの間隔
	int intervalTime = 0;
	//エフェクトタイマー
	int effectTimer = 0;

	//Bullet
	vector<std::unique_ptr<Bullet>> bullet;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy();

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
	/// <param name="playerPosition">自機の座標</param>
	bool Update(XMFLOAT3 playerPosition);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// すべて弾を使っているか
	/// </summary>
	/// <returns>成否</returns>
	bool UsingAllBullet();

	/// <summary>
	/// 始点から終点への距離
	/// </summary>
	/// <param name="pos1">終点</param>
	/// <param name="pos2">始点</param>
	/// <returns>二点間の距離</returns>
	const float Length(XMFLOAT3 pos1, XMFLOAT3 pos2);

public: //アクセッサ
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	XMFLOAT3 GetPosition() { return m_pos; }

	bool GetAlive() { return m_alive; }

	void SetEffectTimer();
};
