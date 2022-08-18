#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Input.h"
#include "Camera.h"
#include "FbxObject3d.h"

class Player
{
public: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //静的メンバ変数
	// 操作系
	static Input* s_input;
	// カメラ
	static Camera* s_camera;

private: // メンバ変数
	// オブジェクト
	std::unique_ptr<FbxObject3d> m_object = nullptr;
	// 座標
	XMFLOAT3 m_pos = {};
	// 生存フラグ
	bool m_alive = false;
	// 加速する
	bool m_isDash = false;
	// 初期加速値
	float m_add0 = 0;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player(FbxObject3d* player);

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
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	XMFLOAT3 GetPosition() { return m_pos; }
};
