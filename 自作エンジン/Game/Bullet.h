#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Object3d.h"

using namespace std;

class Bullet
{
public: // エイリアス
   // DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // メンバ変数
	// オブジェクト
	unique_ptr<Object3d> m_object = nullptr;
	// 座標
	XMFLOAT3 m_pos = { 0, 0, 0 };
	// 速度
	XMFLOAT3 m_vel = { 0, 0, 0 };
	// 生存フラグ
	bool m_alive = false;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Bullet(XMFLOAT3 pos, XMFLOAT3 vec, bool alive);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Bullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="vec">ベクトル</param>
	/// <param name="alive">生存フラグ</param>
	void Initialize(XMFLOAT3 pos, XMFLOAT3 vec, bool alive);

	/// <summary>
	///  更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public: // アクセッサ
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	XMFLOAT3 GetPosition() { return m_pos; }

	/// <summary>
	/// 座標をセット
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(XMFLOAT3 pos);

	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns>ベクトル</returns>
	XMFLOAT3 GetVelocity() { return m_vel; }

	/// <summary>
	/// 速度をセット
	/// </summary>
	/// <param name="vel">ベクトル</param>
	void SetVelocity(XMFLOAT3 vel);

	/// <summary>
	/// 生存フラグを取得
	/// </summary>
	/// <returns>生存フラグ</returns>
	bool GetAlive() { return m_alive; }

	/// <summary>
	/// 生存フラグをセット
	/// </summary>
	/// <param name="alive">生存フラグ</param>
	void SetAlive(bool alive);
};
