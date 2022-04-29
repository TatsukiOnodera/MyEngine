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
	input->Initialize(win);

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

	//スプライト静的初期化
	if (!Sprite::StaticInitialize(dx_cmd->GetDev(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
	}

	//オブジェクト静的初期化
	if (!Object3d::StaticInitialize(dx_cmd->GetDev(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
	}

	//パーティクル静的初期化
	if (!ParticleManager::StaticInitialize(dx_cmd->GetDev(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
	}

	//ライト静的初期化
	if (!Light::StaticInitialize(dx_cmd->GetDev()))
	{
		assert(0);
	}

	//FBXオブジェクト静的初期化
	if (!FbxObject3d::StaticInitialize(dx_cmd->GetDev()))
	{
		assert(0);
	}
}

void FrameWork::Finalize()
{
	//シーンファクトリー開放
	FbxLoader::GetInstance()->Finalize();
	safe_delete(scene_factory);

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
	SceneManager::GetInstance()->Update();
}

void FrameWork::Draw()
{
	dx_cmd->PreDraw();

	SceneManager::GetInstance()->Draw();

	dx_cmd->PostDraw();
}