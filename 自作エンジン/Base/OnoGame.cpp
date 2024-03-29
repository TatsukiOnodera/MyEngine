#include "OnoGame.h"
#include "SceneFactory.h"

void OnoGame::Initialize()
{
	//基底クラスの初期化
	FrameWork::Initialize();

	//シーンの初期化
	scene_factory.reset(new SceneFactory);
	SceneManager::GetInstance()->SetSceneFactory(scene_factory.get());
	//シーンセット
	SceneManager::GetInstance()->ChangeScene("GAME");
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