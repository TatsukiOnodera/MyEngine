#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Player.h"
#include "Bullet.h"

#include <DirectXMath.h>
#include <memory>

class Enemy
{
public: // エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // メンバ変数
	//Enemy
	// オブジェクト
	std::unique_ptr<Object3d> m_enemyOBJ = nullptr;
	// 座標
	XMFLOAT3 m_pos = { 0, 0, 0 };
	// 速度
	XMFLOAT3 m_vel = { 0, 0, 0 };
	// 生存フラグ
	bool m_alive = false;
	// HP
	int m_HP = 0;
	// 重力加速値の時間
	int m_gravityTime = 0;

	//==============================
	// エネミーの弾
	//==============================
	// オブジェクト
	std::vector<std::unique_ptr<Bullet>> enemyBullets;
	// 発射間隔
	int m_bulletInterval = 0;

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
	/// 弾を発射
	/// </summary>
	/// <param name="targetPosition">標的の座標</param>
	void ShotBullet(const XMFLOAT3& targetPos);

	/// <summary>
	/// 使っていない弾があるか
	/// </summary>
	/// <returns>可否</returns>
	bool CheckNoUsingBullet();

	/// <summary>
	/// 射程にいるか
	/// </summary>
	/// <param name="target">対象の座標</param>
	bool SearchTarget(const XMFLOAT3& target);

	/// <summary>
	/// ダメージを受ける
	/// </summary>
	/// <param name="num">ダメージ数</param>
	/// <returns>HPが０か</returns>
	bool EnemyDamage(int num);

	/// <summary>
	/// 地面に着地したら
	/// </summary>
	void CheckCollision();

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
	void SetPosition(const XMFLOAT3& position)
	{
		m_pos = position;

		m_enemyOBJ->SetPosition(m_pos);
	}

	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns>速度</returns>
	XMFLOAT3 GetVelocity() { return m_vel; }

	/// <summary>
	/// 速度をセット
	/// </summary>
	/// <param name="velocity">速度</param>
	void SetVelocity(const XMFLOAT3& velocity)
	{
		m_vel = velocity;
	}


	/// <summary>
	/// 生死フラグの取得
	/// </summary>
	/// <returns>生死フラグ</returns>
	bool GetAlive() { return m_alive; }

	/// <summary>
	/// 生死フラグのセット
	/// </summary>
	/// <param name="alive">成否</param>
	void SetAlive(const bool alive)
	{
		m_alive = alive;
	}

	/// <summary>
	/// オブジェクト情報を取得
	/// </summary>
	/// <returns>オブジェクト</returns>
	Object3d* GetObject3d() { return m_enemyOBJ.get(); }

	/// <summary>
	/// 弾を取得
	/// </summary>
	const std::vector<std::unique_ptr<Bullet>>& GetEnemyBullet() { return enemyBullets; }
};
