#pragma once
#include "Input.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"
#include "Bullet.h"

#include <DirectXMath.h>
#include <memory>

/// <summary>
/// プレイヤー
/// </summary>
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
	// 最大弾数
	static const int c_maxBulletNum = 20;

private: // サブクラス
	// プレイヤーの情報
	struct PlayerInfo
	{
		// HP
		int HP = 20;
		// ブーストゲージ
		int boostGauge = 100;
		// 生存フラグ
		bool isAlive = false;
		// ダッシュフラグ
		bool isDash = false;
		// 座標
		XMFLOAT3 pos = { 0, 0, 0 };
		// 速度
		XMFLOAT3 vel = { 0, 0, 0 };
	};

	//通常 移動パラメータ
	struct MoveParameter
	{
		// 加速度
		float acc = 0;
		// 減速度
		float dec = 0;
		// 最大速度
		float maxVelXZ = 0;
	};

	// ダッシュパラメータ
	struct DashParameter
	{
		// ダッシュの加速度
		float acc = 0;
		// ダッシュの時間
		float time = 0;
		// 1フレーム当たりの加算（DT = DashTime）
		float DT = 0;
	};

	// ジャンプパラメーター
	struct JumpParameter
	{
		// 加速度
		float acc = 0;
		// 最大上昇速度
		float maxVelY = 0;
	};

	// ブースターオフセット
	struct BoosterOffset
	{
		// メインブースター
		XMFLOAT3 mainOffset[2] = {};
		XMFLOAT3 main[2] = {};
		// サイドブースター
		XMFLOAT3 sideOffset[2] = {};
		XMFLOAT3 side[2] = {};
		// バックブースター
		XMFLOAT3 backOffset[2] = {};
		XMFLOAT3 back[2] = {};
	};

	// バレットパラメーター
	struct BulletParameter
	{
		// 標的の座標
		XMFLOAT3 targetPos = { 0, 0, 0 };
		// 装弾数
		int num = 0;
		// 弾速
		float speed = 0;
		// 発射間隔
		int interval = 0;
		// リロードタイム
		int reloadTimer = 0;
		// リロード中か
		bool isReload = false;
		// ロック中か
		bool isLock = false;
	};

	// カメラパラメータ
	struct CameraParameter
	{
		// 回転角度
		float addAngle = 0;
	};

private: // メンバ変数
	//==============================
	// 自機
	//==============================
	// オブジェクト
	std::unique_ptr<Object3d> m_playerOBJ = nullptr;
	// プレイヤー情報
	PlayerInfo m_player;
	// 通常移動
	MoveParameter m_move;
	// ダッシュ
	DashParameter m_dash;
	// ジャンプ
	JumpParameter m_jump;
	// 重力加速値の時間
	int m_gravityTime = 0;

	//==============================
	// プレイヤーの弾
	//==============================
	// オブジェクト
	std::vector<std::unique_ptr<Bullet>> m_playerBullets;
	// モデル
	Model* m_bulletModel = nullptr;
	// バレット
	BulletParameter m_bullet;

	//==============================
	// パーティクル
	//==============================
	// オブジェクト
	std::unique_ptr<ParticleManager> m_booster;
	// オフセット
	BoosterOffset m_offset;

	//==============================
	// カメラ
	//==============================
	CameraParameter m_camera;

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

public: // メンバ関数
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
	/// 速度の正規化
	/// </summary>
	/// <param name="acc">加速値</param>
	void NormalizeVel(XMFLOAT3& acc);

	/// <summary>
	/// プレイヤーのダッシュ
	/// </summary>
	void DashPlayer();

	/// <summary>
	/// 弾を撃つ
	/// </summary>
	void ShotBullet();

	/// <summary>
	/// カメラワーク
	/// </summary>
	void CameraWork();

	/// <summary>
	/// オフセットの更新
	/// </summary>
	void UpdateOffset();

	/// <summary>
	/// メインブースター
	/// </summary>
	void MainBooster();

	/// <summary>
	/// サイドブースター
	/// </summary>
	void SideBooster();

	/// <summary>
	/// 前ブースター
	/// </summary>
	void BacktBooster();

	/// <summary>
	/// ダッシュ時のブースター
	/// </summary>
	void DashBooster(const XMFLOAT3& acc);

	/// <summary>
	/// 地面に着地したら
	/// </summary>
	inline void HitGround()
	{
		m_gravityTime = 0;
		m_player.vel.y = 0;
	}

	/// <summary>
	/// ターゲットを設定
	/// </summary>
	/// <param name="pos">ターゲットの座標</param>
	/// <param name="isTraget">ターゲッティングするか</param>
	inline void SetTarget(bool isTraget, const XMFLOAT3& pos = { 0, 0, 0 })
	{
		m_bullet.isLock = isTraget;
		m_bullet.targetPos = pos;
	}

public: // メンバ関数
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

		m_playerOBJ->SetPosition(m_player.pos);
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
	inline int GetBulletCapacity() { return m_bullet.num; }

	/// <summary>
	/// ダッシュフラグを取得
	/// </summary>
	inline bool GetIsDash() { return m_player.isDash; }

	/// <summary>
	/// FBX取得
	/// </summary>
	inline Object3d* GetPlayerObject() { return m_playerOBJ.get(); }

	/// <summary>
	/// プレイヤーバレットを取得
	/// </summary>
	inline const std::vector<std::unique_ptr<Bullet>>& GetPlayerBullets() { return m_playerBullets; }
};
