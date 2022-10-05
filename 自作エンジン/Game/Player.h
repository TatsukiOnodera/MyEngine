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

private: // メモリ置き場
	// 操作系
	Input* input = Input::GetInstance();

private: // メンバ変数
	//==============================
	// 自機
	//==============================
	// オブジェクト
	unique_ptr<FbxObject3d> m_object = nullptr;
	// 生存フラグ
	bool m_alive = false;

	// 座標
	XMFLOAT3 m_pos = { 0, 0, 0 };
	// 加速値
	XMFLOAT3 m_vel = { 0, 0, 0 };

	// 移動速度
	float m_maxSpeed = 0;

	// 加速する
	bool m_isDash = false;
	// ダッシュ速度
	float m_dashSpeed = 0;
	// ダッシュ時間
	float m_dashTime = 0;

	// ジャンプの最大速度
	float m_maxJumpSpeed = 0;

	// 重力加速値の時間
	int m_gravityTime = 0;

	//==============================
	// 弾
	//==============================
	// オブジェクト
	//vector<unique_ptr<Bullet>> bullet;

	//==============================
	// パーティクル
	//==============================
	std::unique_ptr<ParticleManager> m_booster = nullptr;

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
	void Draw();

	/// <summary>
	/// 始点から終点への距離
	/// </summary>
	/// <param name="pos1">終点</param>
	/// <param name="pos2">始点</param>
	/// <returns>二点間の距離</returns>
	const float Length(XMFLOAT3 pos1, XMFLOAT3 pos2);

public: // アクセッサ
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	XMFLOAT3 GetPosition() { return m_pos; }
	
	/// <summary>
	/// void SetPosition(XMFLOAT3 position);
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position);
};
