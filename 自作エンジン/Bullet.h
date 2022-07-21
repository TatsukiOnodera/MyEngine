#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>

#include "Object3d.h"

class Bullet
{
private: //エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private: //メンバ変数
	//オブジェクトクラス
	std::unique_ptr< Object3d> m_bullet = nullptr;
	//座標
	XMFLOAT3 m_pos = {};
	//ベクトル
	XMFLOAT3 m_vec = {};
	//生存フラグ
	bool m_alive = false;

public: //メンバ関数
	//コンストラクタ
	Bullet(XMFLOAT3 pos, XMFLOAT3 vec);

	//デストラクタ
	~Bullet();

	//初期化
	void Initialize(XMFLOAT3 pos, XMFLOAT3 vec);

	//更新
	void Update();

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//生存フラグを取得
	bool GetAlive() { return m_alive; }

	//生存フラグをセット
	void activeBullet(XMFLOAT3 pos, XMFLOAT3 vec);
};
