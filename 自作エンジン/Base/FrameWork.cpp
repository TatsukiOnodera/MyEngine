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
	win = WinApp::GetInstance();
	win->CreateGameWindow();

	//Input初期化
	input = Input::GetInstance();
	input->Initialize(win);

	//カメラ初期化
	Camera::GetInstance()->Initialize();

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

	//ポストエフェクト静的初期化
	if (!PostEffect::StaticInitialize(dx_cmd->GetDev(), WinApp::window_width, WinApp::window_height))
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

<<<<<<< HEAD
	/*postEffect.reset(new PostEffect);
	postEffect->Initialize();*/
=======
	//Sprite::LoadTexture(100, L"Resources/Default/white1x1.png");

	postEffect.reset(new PostEffect);
	postEffect->Initialize();
>>>>>>> parent of 00f20c5... no message
}

void FrameWork::Finalize()
{
	// ウィンドウクラスを登録解除
	win->TerminateGameWindow();
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
	//レンダーテクスチャへの描画
<<<<<<< HEAD
	//postEffect->PreDrawScene(dx_cmd->GetCmdList());

	//SceneManager::GetInstance()->Draw();

	//postEffect->PostDrawScene(dx_cmd->GetCmdList());

	//描画開始
	dx_cmd->PreDraw();
=======
	postEffect->PreDrawScene(dx_cmd->GetCmdList());
>>>>>>> parent of 00f20c5... no message

	//postEffect->Draw(dx_cmd->GetCmdList());
	SceneManager::GetInstance()->Draw();

	postEffect->PostDrawScene(dx_cmd->GetCmdList());

	//描画開始
	dx_cmd->PreDraw();

	postEffect->Draw(dx_cmd->GetCmdList());

	dx_cmd->PostDraw();
}