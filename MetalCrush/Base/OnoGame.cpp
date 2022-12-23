#include "OnoGame.h"
#include "SceneFactory.h"
#include "Sprite.h"
#include "Object3d.h"

void OnoGame::LoadResources()
{
	// スプライトテクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/Reticle.png");
	Sprite::LoadTexture(2, L"Resources/NumberText.png");
	Sprite::LoadTexture(3, L"Resources/HPFrame.png");
	Sprite::LoadTexture(4, L"Resources/HPGauge.png");
	Sprite::LoadTexture(5, L"Resources/BoosterFrame.png");
	Sprite::LoadTexture(6, L"Resources/BoosterGauge.png");
	Sprite::LoadTexture(7, L"Resources/Monitor.png");
	Sprite::LoadTexture(8, L"Resources/GameOverScreen.png");
	Sprite::LoadTexture(9, L"Resources/BulletNumFrame.png");
	Sprite::LoadTexture(10, L"Resources/Rader.png");
	Sprite::LoadTexture(11, L"Resources/RaderEnemy.png");
	Sprite::LoadTexture(12, L"Resources/startScreen.png");
	Sprite::LoadTexture(13, L"Resources/EndScreen.png");

	// OBJモデル読み込み
	Object3d::LoadModel(0, "Desert", true);
	Object3d::LoadModel(1, "SkyWall");
	Object3d::LoadModel(2, "Tank");
	Object3d::LoadModel(3, "Bullet", true);
	Object3d::LoadModel(4, "Leg");
	Object3d::LoadModel(5, "Body");
	Object3d::LoadModel(6, "Arm");

	// FBXモデル読み込み

}

void OnoGame::Initialize()
{
	//基底クラスの初期化
	FrameWork::Initialize();

	//シーンの初期化
	scene_factory.reset(new SceneFactory);
	SceneManager::GetInstance()->SetSceneFactory(scene_factory.get());
	//シーンセット
	SceneManager::GetInstance()->ChangeScene("GAME");

	// リソースのロード
	LoadResources();
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