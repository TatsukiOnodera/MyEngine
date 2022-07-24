#pragma once
#include "BaseObject.h"
#include "FbxObject3d.h"

class Player : public BaseObject
{
private: //メモリ置き場
	//コマンドリスト
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCmdList();
	//操作系
	Input* input = Input::GetInstance();
	//オーディオ
	Audio* audio = Audio::GetInstance();
	//カメラ
	Camera* camera = Camera::GetInstance();
	//デバッグテキスト
	DebugText debugText;

private: //メンバ変数
	//オブジェクト
	std::unique_ptr<FbxObject3d> m_player = nullptr;

	//加速する
	bool m_isDash;
	//初期加速値
	float m_add0;

public: //メンバ関数
	//デストラクタ
	~Player() override;

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;
};
