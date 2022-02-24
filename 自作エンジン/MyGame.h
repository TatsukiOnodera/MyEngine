#pragma once
#include "WinApp.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Camera.h"
#include "GameScene.h"
#include "ParticleManager.h"
#include "SafeDelete.h"

class MyGame
{
private: //ポインタ置き場
	WinApp* win = nullptr;
	Input* input = nullptr;
	Camera* camera = nullptr;
	Audio* audio = nullptr;
	DirectXCommon* dx_cmd = nullptr;
	GameScene* game_scene = nullptr;

private: //その他変数
	bool end_request = false;

public: //関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了リクエストの成否
	/// </summary>
	bool IsEndRequest() { return end_request; }
};