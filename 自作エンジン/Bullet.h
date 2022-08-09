#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Object3d.h"

class Bullet
{
public: // エイリアス
   // DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //メンバ変数
	//オブジェクト
	std::unique_ptr<Object3d> m_object = nullptr;
	//移動ベクトル
	XMFLOAT3 m_vec = { 0, 0, 0 };
	//生存フラグ
	bool m_alive = false;

public: //メンバ関数
	//コンストラクタ
	Bullet(Object3d* bullet);

	//デストラクタ
	~Bullet();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//座標を取得
	XMFLOAT3 GetPosition() { return m_object->GetPosition(); }

	//座標をセット
	void SetPosition(XMFLOAT3 pos);

	//移動ベクトルを取得
	XMFLOAT3 GetVector() { return m_vec; }

	//移動ベクトルをセット
	void SetVector(XMFLOAT3 vec);

	//生存フラグを取得
	bool GetAlive() { return m_alive; }

	//生存フラグをセット
	void SetAlive(bool alive);
};
