#include "MyGame.h"

void MyGame::Initialize()
{
	//windowsアプリケーションの初期化
	win = new WinApp;
	win->CreateGameWindow();

	//Input初期化
	input = new Input;
	input->Initialize(win->GetWindowInstance(), win->GetHWND());

	//カメラ初期化
	camera = new Camera;
	camera->Initialize(WinApp::window_width, WinApp::window_height);

	//オーディオ初期化
	audio = new Audio;

	// DirectX初期化処理
	dx_cmd = new DirectXCommon();
	if (!dx_cmd->Initialize(win))
	{
		assert(0);
	}

	//Sprite初期化
	if (!Sprite::StaticInitialize(dx_cmd->GetDev(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
	}

	//Object3d初期化
	if (!Object3d::StaticInitialize(dx_cmd->GetDev(), camera, WinApp::window_width, WinApp::window_height))
	{
		assert(0);
	}

	//パーティクル初期化
	if (!ParticleManager::StaticInitialize(dx_cmd->GetDev(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
	}

	//ゲームシーン初期化
	game_scene = new GameScene;
	game_scene->Initialize(dx_cmd, input, audio, camera);
}

void MyGame::Finalize()
{
	//開放
	safe_delete(game_scene);
	safe_delete(dx_cmd);
	safe_delete(camera);
	safe_delete(input);
	safe_delete(audio);

	// ウィンドウクラスを登録解除
	win->TerminateGameWindow();
	safe_delete(win);
}

void MyGame::Update()
{
	//終了リクエスト
	if (win->ProcessMessage() || input->PushKey(DIK_ESCAPE))
	{
		end_request = true;
	}

	//キー情報取得
	input->Update();
	
	//ゲームシーン
	game_scene->Update();
}

void MyGame::Draw()
{
	dx_cmd->PreDraw();

	game_scene->Draw();

	dx_cmd->PostDraw();
}