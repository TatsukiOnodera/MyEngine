#pragma once
#include "Input.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"

#include "Bullet.h"

#include <DirectXMath.h>
#include <memory>

class Player
{
public: // エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // デバイス
	// カメラ
	static Camera* s_camera;
	// 操作系
	static Input* s_input;

private: // 定数
	// 通常移動
	// 加速度
	const float c_accMove = 0.0325f;
	// 減速度
	const float c_decMove = 0.9f;
	// 最大速度
	const float c_maxVelXZ = 0.6f;

	// ジャンプ
	// 加速度
	const float c_accJump = 0.05f;
	// 最大速度
	const float c_maxVelY = 0.6f;

	// カメラ
	// 回転角度
	const float c_addAngle = 1.0f;

private: // サブクラス
	// プレイヤーの情報
	struct PlayerInfo
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
	std::unique_ptr<FbxObject3d> m_playerFBX = nullptr;
	// プレイヤー情報
	PlayerInfo m_player;
	// ダッシュの加速度
	float m_dashAcc = 0;
	// ダッシュの時間
	float m_dashTime = 0;
	// 1フレーム当たりの加算（DT = DashTime）
	float m_addDT = 10.0f;
	// 重力加速値の時間
	int m_gravityTime = 0;

	//==============================
	// プレイヤーの弾
	//==============================
	// オブジェクト
	std::vector<std::unique_ptr<Bullet>> m_playerBullets;
	// モデル
	Model* m_bulletModel = nullptr;
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
	// パーティクル
	//==============================
	std::unique_ptr<ParticleManager> m_booster;
	XMFLOAT3 m_mBoosterPos[2] = {};
	XMFLOAT3 m_sBoosterPos[2] = {};
	XMFLOAT3 m_bBoosterPos[2] = {};

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player(Model* playerModel, Model* bulletModel);

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

	void UpdateOffset();

	/// <summary>
	/// 背面ブースター
	/// </summary>
	void MainBooster(const XMFLOAT3& acc);

	/// <summary>
	/// 横ブースター
	/// </summary>
	void SideBooster(const XMFLOAT3& acc);

	/// <summary>
	/// 前ブースター
	/// </summary>
	void BacktBooster(const XMFLOAT3& acc);

	void DashBooster(const XMFLOAT3& acc);

	/// <summary>
	/// 始点から終点への距離
	/// </summary>
	/// <param name="pos1">終点</param>
	/// <param name="pos2">始点</param>
	/// <returns>二点間の距離</returns>
	const float Length(const XMFLOAT3& pos1, const XMFLOAT3& pos2 = { 0, 0, 0 });

	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="tmp">正規化する値</param>
	/// <returns>正規化した値</returns>
	const XMFLOAT3 normalize(const XMFLOAT3& tmp);

public: // アクセッサ
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	inline const XMFLOAT3& GetPosition() { return m_player.pos; }
	
	/// <summary>
	/// void SetPosition(XMFLOAT3 position);
	/// </summary>
	/// <param name="position">座標</param>
	inline void SetPosition(const XMFLOAT3& position)
	{
		m_player.pos = position;

		m_playerFBX->SetPosition(m_player.pos);
	}

	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns>速度</returns>
	inline const XMFLOAT3& GetVelocity() { return m_player.vel; }

	/// <summary>
	/// 生死フラグを取得
	/// </summary>
	inline bool GetAlive() { return m_player.isAlive; }

	/// <summary>
	/// 生死フラグをセット
	/// </summary>
	/// <param name="alive">生死フラグ</param>
	inline void SetAlive(const bool alive)
	{
		m_player.isAlive = alive;
	}

	/// <summary>
	/// 標的の座標をセット
	/// </summary>
	/// <param name="targetPosition">標的の座標</param>
	inline void SetTargetPosition(const XMFLOAT3& targetPosition)
	{
		m_targetPos = targetPosition;
	}

	/// <summary>
	/// ロックフラグをセット
	/// </summary>
	inline void SetIsLock(const bool isLock)
	{
		m_isLock = isLock;
	}

	/// <summary>
	/// HPの取得
	/// </summary>
	/// <returns>HP</returns>
	inline int GetPlayerHP() { return m_player.HP; }

	/// <summary>
	/// HPのセット
	/// </summary>
	/// <param name="m_HP">HP</param>
	inline void SetPlayerHP(const int HP)
	{
		m_player.HP = HP;
	}

	/// <summary>
	/// 装弾数取得
	/// </summary>
	/// <returns>装弾数</returns>
	inline int GetBulletCapacity() { return m_bulletCapacity; }

	/// <summary>
	/// ダッシュフラグを取得
	/// </summary>
	inline bool GetIsDash() { return m_player.isDash; }

	/// <summary>
	/// FBX取得
	/// </summary>
	inline FbxObject3d* GetPlayerObject() { return m_playerFBX.get(); }

	/// <summary>
	/// 弾を取得
	/// </summary>
	inline const std::vector<std::unique_ptr<Bullet>>& GetPlayerBullets() { return m_playerBullets; }
};
