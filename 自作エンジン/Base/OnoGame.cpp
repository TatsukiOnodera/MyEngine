#include "OnoGame.h"
#include "GamePlayScene.h"
#include "TitleScene.h"

void OnoGame::Initialize()
{
	//基底クラスの初期化
	FrameWork::Initialize();

	//シーンの初期化
	BaseScene* scene = new TitleScene;
	//シーンセット
	SceneManager::GetInstance()->SetNextScene(scene);
}

void OnoGame::Finalize()
{
	//基底クラスの初期化
	FrameWork::Finalize();
}

void OnoGame::Update()
{
	//基底クラスの更新
	FrameWork::Update();
}

void OnoGame::Draw()
{
	//基底クラスの更新
	FrameWork::Draw();
}