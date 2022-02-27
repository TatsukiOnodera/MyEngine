#include "OnoGame.h"
#include "GamePlayScene.h"

void OnoGame::Initialize()
{
	//基底クラスの初期化
	FrameWork::Initialize();

	//シーンの初期化
	game_scene = new GamePlayScene;
	game_scene->Initialize(dx_cmd, input, audio, camera);
}

void OnoGame::Finalize()
{
	//シーンの開放
	safe_delete(game_scene);
	
	//基底クラスの初期化
	FrameWork::Finalize();
}

void OnoGame::Update()
{
	//基底クラスの更新
	FrameWork::Update();
	
	//シーンの更新
	game_scene->Update();
}

void OnoGame::Draw()
{
	dx_cmd->PreDraw();

	game_scene->Draw();

	dx_cmd->PostDraw();
}