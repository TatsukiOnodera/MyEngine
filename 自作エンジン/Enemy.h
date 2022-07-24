#pragma once
#include "BaseObject.h"

class Enemy : public BaseObject
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


public: //メンバ関数
	//デストラクタ
	~Enemy() override;

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;
};
