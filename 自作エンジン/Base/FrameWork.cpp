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
}

void FrameWork::Finalize()
{
	//開放
	safe_delete(dx_cmd);
	safe_delete(camera);
	safe_delete(input);
	safe_delete(audio);

	// ウィンドウクラスを登録解除
	win->TerminateGameWindow();
	safe_delete(win);
}

void FrameWork::Update()
{
	//終了リクエスト
	if (win->ProcessMessage() || input->PushKey(DIK_ESCAPE))
	{
		end_request = true;
	}

	//キー情報取得
	input->Update();
}

void FrameWork::Draw()
{

}