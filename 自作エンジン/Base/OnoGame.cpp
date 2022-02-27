#include "OnoGame.h"
#include "GamePlayScene.h"
#include "GameTitleScene.h"

void OnoGame::Initialize()
{
	//基底クラスの初期化
	FrameWork::Initialize();

	//ゲームシーン初期化
	BaseScene *scene = new GameTitleScene(dx_cmd, input, audio, camera, scene_manager);
	//シーンマネージャーにシーンをセット
	scene_manager->SetNextScene(scene);
}

void OnoGame::Finalize()
{
	//基底クラスの開放
	FrameWork::Finalize();
}

void OnoGame::Update()
{
	FrameWork::Update();
}

void OnoGame::Draw()
{
	FrameWork::Draw();
}