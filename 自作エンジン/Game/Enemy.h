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

private: // メンバ変数
	//Enemy
	// オブジェクト
	std::unique_ptr<Object3d> m_object = nullptr;
	// 座標
	XMFLOAT3 m_pos = { 0, 0, 50 };
	// 速度
	XMFLOAT3 m_vel = { 0, 0, 0 };
	// 生存フラグ
	bool m_alive = false;

	//==============================
	// エネミーの弾
	//==============================
	// オブジェクト
	std::vector<std::unique_ptr<Bullet>> enemyBullets;
	// 標的の座標
	XMFLOAT3 m_targetPos = { 0, 0, 0 };

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
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 衝突時のコールバック関数
	/// </summary>
	void OnCollision();

public: //アクセッサ
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	XMFLOAT3 GetPosition() { return m_pos; }

	/// <summary>
	/// 座標をセット
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const XMFLOAT3& position);

	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns>速度</returns>
	XMFLOAT3 GetVelocity() { return m_vel; }

	/// <summary>
	/// 速度をセット
	/// </summary>
	/// <param name="velocity">速度</param>
	void SetVelocity(const XMFLOAT3& velocity);

	/// <summary>
	/// 生死フラグの取得
	/// </summary>
	/// <returns>生死フラグ</returns>
	bool GetAlive() { return m_alive; }

	/// <summary>
	/// 生死フラグのセット
	/// </summary>
	/// <param name="alive">成否</param>
	void SetAlive(const bool& alive);

	/// <summary>
	/// 弾を取得
	/// </summary>
	const std::vector<std::unique_ptr<Bullet>>& GetEnemyBullet() { return enemyBullets; }
};
