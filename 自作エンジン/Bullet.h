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

public: // 静的メンバ関数
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="vec">ベクトル</param>
	/// <param name="alive">生存フラグ</param>
	/// <returns>Bullet</returns>
	static Bullet* Create(XMFLOAT3 pos, XMFLOAT3 vec, bool alive);

private: // メンバ変数
	// オブジェクト
	unique_ptr<Object3d> m_object = nullptr;
	// 座標
	XMFLOAT3 m_pos = { 0, 0, 0 };
	// 移動ベクトル
	XMFLOAT3 m_vec = { 0, 0, 0 };
	// 生存フラグ
	bool m_alive = false;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Bullet();

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
	bool Update(XMFLOAT3 pos);

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
	/// 座標をセット
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(XMFLOAT3 pos);

	/// <summary>
	/// 移動ベクトルを取得
	/// </summary>
	/// <returns>ベクトル</returns>
	XMFLOAT3 GetVector() { return m_vec; }

	/// <summary>
	/// 移動ベクトルをセット
	/// </summary>
	/// <param name="vec">ベクトル</param>
	void SetVector(XMFLOAT3 vec);

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
