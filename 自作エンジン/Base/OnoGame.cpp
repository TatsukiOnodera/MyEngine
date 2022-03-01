#include "OnoGame.h"
#include "GamePlayScene.h"
#include "TitleScene.h"

void OnoGame::Initialize()
{
	//基底クラスの初期化
	FrameWork::Initialize();

	//シーンの初期化
	scene = new TitleScene;
	scene->Initialize(dx_cmd, input, audio, camera);
}

void OnoGame::Finalize()
{
	//シーンの開放
	safe_delete(scene);
	
	//基底クラスの初期化
	FrameWork::Finalize();
}

void OnoGame::Update()
{
	//基底クラスの更新
	FrameWork::Update();
	
	//シーンの更新
	scene->Update();
}

void OnoGame::Draw()
{
	dx_cmd->PreDraw();

	scene->Draw();

	dx_cmd->PostDraw();
}