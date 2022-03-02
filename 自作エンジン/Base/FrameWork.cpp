#include "FrameWork.h"

void FrameWork::Run()
{
	//ゲーム初期化
	Initialize();

	while (1)  // ゲームループ
	{
		//ゲーム毎フレーム更新
		Update();

		//ゲーム終了リクエスト
		if (end_request)
		{
			break;
		}

		//ゲーム描画
		Draw();
	}

	//ゲーム終了
	Finalize();
}

void FrameWork::Initialize()
{
	//windowsアプリケーションの初期化
	win = new WinApp;
	win->CreateGameWindow();

	//Input初期化
	input = Input::GetInstance();
	input->Initialize(win->GetWindowInstance(), win->GetHWND());

	//カメラ初期化
	camera = Camera::GetInstance();
	camera->Initialize(WinApp::window_width, WinApp::window_height);

	//オーディオ初期化
	audio = Audio::GetInstance();

	// DirectX初期化処理
	dx_cmd = DirectXCommon::GetInstance();
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

	//シーンマネージャー生成
	scene_manager = new SceneManager;
}

void FrameWork::Finalize()
{
	//開放
	safe_delete(scene_manager);

	// ウィンドウクラスを登録解除
	win->TerminateGameWindow();
	safe_delete(win);
}

void FrameWork::Update()
{
	//終了のリクエスト
	if (win->ProcessMessage() || input->PushKey(DIK_ESCAPE))
	{
		end_request = true;
	}

	//キーの情報取得
	input->Update();

	//シーンの更新
	scene_manager->Update();
}

void FrameWork::Draw()
{
	dx_cmd->PreDraw();

	scene_manager->Draw();

	dx_cmd->PostDraw();
}