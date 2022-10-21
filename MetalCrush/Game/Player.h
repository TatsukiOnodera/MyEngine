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

private: // サブクラス
	// プレイヤーのステータス
	struct PlayerStatus
	{
		// HP
		int HP = 20;

		// 生存フラグ
		bool isAlive = false;
		// ダッシュフラグ
		bool isDash = false;

		// 座標
		XMFLOAT3 pos = { 0, 0, 0 };
		// 速度
		XMFLOAT3 vel = { 0, 0, 0 };
	};

private: // メンバ変数
	//==============================
	// 自機
	//==============================
	// オブジェクト
	unique_ptr<FbxObject3d> m_object = nullptr;
	// ステータス
	PlayerStatus m_status;

	// 通常移動の加速度
	const float c_accMove = 0.0325f;
	// 通常移動の減速度
	const float c_decMove = 0.9f;
	// 通常移動の最大速度
	const float c_maxVelXZ = 0.6f;
	// ジャンプの加速度
	const float c_accJump = 0.05f;
	// ジャンプの最大速度
	const float c_maxVelY = 0.6f;

	// ダッシュの加速度
	float m_dashAcc = 0;
	// ダッシュの時間
	float m_dashTime = 0;
	// 1フレーム当たりの加算（DT = DashTime）
	float m_addDT = 6.0f;
	// 重力加速値の時間
	int m_gravityTime = 0;

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
	// 装弾数
	int m_bulletCapacity = 0;
	// リロードタイム
	int m_reloadTimer = 0;

	//==============================
	// カメラ
	//==============================
	// カメラの回転角度
	const float c_addAngle = 0;
	// カメラ位置の正面化
	bool m_cameraInitialize = false;

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
	void CameraWork();

	/// <summary>
	/// 地面に着地したら
	/// </summary>
	void OnLand();

public: // アクセッサ
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3 GetPosition() { return m_status.pos; }
	
	/// <summary>
	/// void SetPosition(XMFLOAT3 position);
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const XMFLOAT3& position)
	{
		m_status.pos = position;

		m_object->SetPosition(m_status.pos);
	}

	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns>速度</returns>
	const XMFLOAT3 GetVelocity() { return m_status.vel; }

	/// <summary>
	/// 生死フラグを取得
	/// </summary>
	bool GetAlive() { return m_status.isAlive; }

	/// <summary>
	/// 生死フラグをセット
	/// </summary>
	/// <param name="alive">生死フラグ</param>
	void SetAlive(const bool alive)
	{
		m_status.isAlive = alive;
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
	int GetPlayerHP() { return m_status.HP; }

	/// <summary>
	/// HPのセット
	/// </summary>
	/// <param name="m_HP">HP</param>
	void SetPlayerHP(const int HP)
	{
		m_status.HP = HP;
	}

	/// <summary>
	/// 装弾数取得
	/// </summary>
	/// <returns>装弾数</returns>
	int GetBulletCapacity() { return m_bulletCapacity; }

	/// <summary>
	/// ダッシュフラグを取得
	/// </summary>
	bool GetIsDash() { return m_status.isDash; }

	/// <summary>
	/// 弾を取得
	/// </summary>
	const std::vector<std::unique_ptr<Bullet>>& GetPlayerBullets() { return playerBullets; }
};
