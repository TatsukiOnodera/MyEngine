#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Input.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "Bullet.h"
#include "Enemy.h"

using namespace std;

class Player
{
public: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private: //静的メンバ変数
	// 操作系
	static Input* s_input;
	// カメラ
	static Camera* s_camera;

public: //静的メンバ関数
	/// <summary>
	/// 生成
	/// </summary>
	/// <returns>Player</returns>
	static Player* Create();

private: // メンバ変数
	// Player
	// オブジェクト
	unique_ptr<FbxObject3d> m_object = nullptr;
	// 座標
	XMFLOAT3 m_pos = { 0, 0, -50 };
	// 生存フラグ
	bool m_alive = false;
	// 加速する
	bool m_isDash = false;
	// 初期加速値
	float m_add0 = 0;
	//エフェクトタイマー
	int effectTimer = 0;

	// Bullet
	// オブジェクト
	vector<unique_ptr<Bullet>> bullet;

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
	bool Update(XMFLOAT3 enemyPos);

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

public: // アクセッサ
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	XMFLOAT3 GetPosition() { return m_pos; }

	void SetEffectTimer();
};
