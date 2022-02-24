#pragma once
#include "WinApp.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Camera.h"
#include "GameScene.h"
#include "ParticleManager.h"
#include "SafeDelete.h"

class FrameWork
{
protected: //ポインタ置き場
	WinApp* win = nullptr;
	Input* input = nullptr;
	Camera* camera = nullptr;
	Audio* audio = nullptr;
	DirectXCommon* dx_cmd = nullptr;

public: //その他変数
	bool end_request = false;

public:
	/// <summary>
	/// 実行
	/// </summary>
	void Run();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();
};