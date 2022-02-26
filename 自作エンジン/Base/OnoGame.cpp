#include "OnoGame.h"
#include "GamePlayScene.h"

void OnoGame::Initialize()
{
	//基底クラスの初期化
	FrameWork::Initialize();

	//ゲームシーン初期化
	game_scene = new GamePlayScene;
	game_scene->Initialize(dx_cmd, input, audio, camera);
}

void OnoGame::Finalize()
{
	//開放
	safe_delete(game_scene);
	
	//基底クラスの開放
	FrameWork::Finalize();
}

void OnoGame::Update()
{
	FrameWork::Update();
	
	//ゲームシーン
	game_scene->Update();
}

void OnoGame::Draw()
{
	dx_cmd->PreDraw();

	game_scene->DrawBackSprite(); //前景スプライト
	game_scene->Draw(); //オブジェクト＆スプライト描画
	//game_scene->DrawParticles(); //パーティクル描画
	game_scene->DrawUI(); //UI描画

	game_scene->DrawDebugText(); //デバッグテキスト描画

	dx_cmd->PostDraw();
}