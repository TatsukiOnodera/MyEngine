#include "WinApp.h"
#include "Input.h"
#include "Camera.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "PArticleManager.h"
#include "SafeDelete.h"

//# Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WinApp* win = nullptr;
	Input* input = nullptr;
	DirectXCommon* dx_cmd = nullptr;
	Audio* audio = new Audio;
	Camera* camera = nullptr;
	GameScene* gameScene = new GameScene;

	//windowsアプリケーションの初期化
	win = new WinApp;
	win->CreateGameWindow();

	//Input初期化
	input = new Input;
	input->Initialize(win->GetWindowInstance(), win->GetHWND());

	//カメラ初期化
	camera = new Camera;
	camera->Initialize(WinApp::window_width, WinApp::window_height);

	// DirectX初期化処理
	dx_cmd = new DirectXCommon();
	if (!dx_cmd->Initialize(win))
	{
		assert(0);
		return 1;
	}
	
	//Sprite初期化
	if (!Sprite::StaticInitialize(dx_cmd->GetDev(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
		return 1;
	}

	//Object3d初期化
	if (!Object3d::StaticInitialize(dx_cmd->GetDev(), camera, WinApp::window_width, WinApp::window_height))
	{
		assert(0);
		return 1;
	}

	//パーティクル初期化
	if (!ParticleManager::StaticInitialize(dx_cmd->GetDev(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
		return 1;
	}

	//ゲームシーン初期化
	gameScene->Initialize(dx_cmd, input, audio, camera);

	while (true)  // ゲームループ
	{
		if (win->ProcessMessage())
		{
			break;
		}

		//キー情報取得
		input->Update();

		//ゲームシーン
		gameScene->Update();

		//ここから描画
		dx_cmd->PreDraw();

		gameScene->Draw();

		dx_cmd->PostDraw();
	}

	//開放
	safe_delete(gameScene);
	safe_delete(input);
	safe_delete(camera);
	safe_delete(audio);
	safe_delete(dx_cmd);
	// ウィンドウクラスを登録解除
	win->TerminateGameWindow();
	safe_delete(win);

	return 0;
}