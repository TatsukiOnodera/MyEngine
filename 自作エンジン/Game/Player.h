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
	// 座標
	XMFLOAT3 m_pos = { 0, 0, 0 };
	// 速度
	XMFLOAT3 m_vel = { 0, 0, 0 };
	// 毎加速度
	float m_accSpeed = 0;
	// 減速度
	float m_decSpeed = 0;
	// ジャンプ加速度
	float m_accJumpSpeed = 0;
	// 重力加速値の時間
	int m_gravityTime = 0;
	// ダッシュフラグ
	bool m_isDash = false;
	// ダッシュ加速比
	float m_dashTimes = 0;
	// ダッシュ加速比の毎加算値
	float m_accDashTimes = 0;
	// カメラの角度
	XMFLOAT3 m_angle = { 360, 0, 0 };
	// カメラの回転角度
	float m_addAngle = 0;
	// カメラ位置の正面化
	bool m_cameraInitialize = false;

	//==============================
	// 自機の弾
	//==============================


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
	/// カメラ位置の初期化
	/// </summary>
	bool CameraInitialize(float& anglex, float& angleY);

public: // 衝突
	/// <summary>
	/// 衝突時のコールバック関数
	/// </summary>
	void OnCollision();

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
	void SetPosition(const XMFLOAT3& position);

	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns>速度</returns>
	XMFLOAT3 GetVelocity() { return m_vel; }

	/// <summary>
	/// 生死フラグを取得
	/// </summary>
	bool GetAlive() { return m_alive; }
};
