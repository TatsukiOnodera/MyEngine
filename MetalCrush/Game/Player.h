#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Input.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"

#include "Bullet.h"

using namespace std;

class Player
{
public: // エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	// カメラ
	static Camera* s_camera;
	// 操作系
	static Input* s_input;

private: // メンバ変数
	//==============================
	// 自機
	//==============================
	// オブジェクト
	unique_ptr<FbxObject3d> m_object = nullptr;
	// 生存フラグ
	bool m_alive = false;
	// HP
	int m_HP = 20;
	// 座標
	XMFLOAT3 m_pos = { 0, 0, 0 };
	// 速度
	XMFLOAT3 m_vel = { 0, 0, 0 };
	// 毎加速度
	float m_accSpeed = 0;
	// 減速度
	float m_decSpeed = 0;
	// ジャンプ加速度
	float m_accJump = 0;
	// 重力加速値の時間
	int m_gravityTime = 0;
	// ダッシュフラグ
	bool m_isDash = false;
	// ダッシュ加速比
	float m_dashTimes = 0;
	// ダッシュ加速比の毎加算値
	float m_accDashTimes = 0;
	// カメラの回転角度
	float m_addAngle = 0;
	// カメラ位置の正面化
	bool m_cameraInitialize = false;

	//==============================
	// プレイヤーの弾
	//==============================
	// オブジェクト
	std::vector<std::unique_ptr<Bullet>> playerBullets;
	// 標的の座標
	XMFLOAT3 m_targetPos = { 0, 0, 0 };
	// ロック中か
	bool m_isLock = false;
	// 発射間隔
	int m_shotInterval = 0;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

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
	/// プレイヤーの移動
	/// </summary>
	/// <param name="acc">加速値</param>
	void MovePlayer(XMFLOAT3& acc);

	/// <summary>
	/// プレイヤーのジャンプ
	/// </summary>
	/// <param name="acc">加速値</param>
	void JumpPlayer(XMFLOAT3& acc);

	/// <summary>
	/// 速度に加速度を加算
	/// </summary>
	/// <param name="acc">加速値</param>
	void AddAcceleration(const XMFLOAT3& acc);

	/// <summary>
	/// プレイヤーのダッシュ
	/// </summary>
	void DashPlayer();

	/// <summary>
	/// 弾を撃つ
	/// </summary>
	void ShotBullet();

	/// <summary>
	/// 使っていない弾があるか
	/// </summary>
	/// <returns>可否</returns>
	bool CheckNoUsingBullet();

	/// <summary>
	/// カメラワーク
	/// </summary>
	void MoveCamera();

	/// <summary>
	/// 地面に着地したら
	/// </summary>
	void OnLand();

public: // アクセッサ
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3 GetPosition() { return m_pos; }
	
	/// <summary>
	/// void SetPosition(XMFLOAT3 position);
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const XMFLOAT3& position)
	{
		m_pos = position;

		m_object->SetPosition(m_pos);
	}

	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns>速度</returns>
	const XMFLOAT3 GetVelocity() { return m_vel; }

	/// <summary>
	/// 生死フラグを取得
	/// </summary>
	bool GetAlive() { return m_alive; }

	/// <summary>
	/// 生死フラグをセット
	/// </summary>
	/// <param name="alive">生死フラグ</param>
	void SetAlive(const bool alive)
	{
		m_alive = alive;
	}

	/// <summary>
	/// 標的の座標をセット
	/// </summary>
	/// <param name="targetPosition">標的の座標</param>
	void SetTargetPosition(const XMFLOAT3& targetPosition)
	{
		m_targetPos = targetPosition;
	}

	/// <summary>
	/// ロックフラグをセット
	/// </summary>
	void SetIsLock(const bool isLock)
	{
		m_isLock = isLock;
	}

	/// <summary>
	/// HPの取得
	/// </summary>
	/// <returns>HP</returns>
	int GetPlayerHP() { return m_HP; }

	/// <summary>
	/// HPのセット
	/// </summary>
	/// <param name="m_HP">HP</param>
	void SetPlayerHP(const int HP)
	{
		m_HP = HP;
	}

	/// <summary>
	/// ダッシュフラグを取得
	/// </summary>
	bool GetIsDash() { return m_isDash; }

	/// <summary>
	/// 弾を取得
	/// </summary>
	const std::vector<std::unique_ptr<Bullet>>& GetPlayerBullets() { return playerBullets; }
};
