#include "GamePlayScene.h"
#include "SceneManager.h"

#include <time.h>
#include <stdlib.h>
#include <SafeDelete.h>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <fbxsdk.h>

using namespace DirectX;
using namespace Microsoft::WRL;

GamePlayScene::~GamePlayScene()
{
	safe_delete(light);
}

void GamePlayScene::Initialize()
{
	dx_cmd = DirectXCommon::GetInstance();
	fbxLoader = FbxLoader::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	camera = Camera::GetInstance();
	
	//スプライトテクスチャ読み込み
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/background.png");

	//FBXの読み込み
	fbxLoader->LoadModelFromFile("cube");

	//前景スプライト
	debugText.Initialize(fontNumber);

	//ライト生成
	light = Light::Create();
	light->SetLightColor({ 1, 1, 1 });
	Object3d::SetLight(light);

	//スプライト
	demo_back.reset(Sprite::CreateSprite(1));

	//オブジェクト
	gravity.reset(Object3d::Create("Bullet", true));
	bullet.reset(Object3d::Create("Bullet", true));

	//パラメーター
	ResetVariable();

	//オーディオ
	audio->Initialize();
}

void GamePlayScene::ResetVariable()
{
	light->SetLightDir({-10, -10, 0, 1});

	gravityTime = 0;
	bulletTimeX= 0;
	bulletTimeY = 0;

	gravityAcc = 0;
	bulletAcc = {2, 1, 0};

	camera->SetEye({ 0, 0, -50 });
	camera->MoveCamera({20, 0, 0});

	gravity->SetPosition({ -15, 25, 0 });
	gravity->Update();

	bullet->SetPosition({ 0, 0, 0 });
	bullet->Update();
}

void GamePlayScene::Update()
{
	XMFLOAT3 gravityPos = gravity->GetPosition();
	XMFLOAT3 bulletPos = bullet->GetPosition();

	//自由落下
	if (gravityPos.y == -25)
	{
		gravityPos.y = -gravityPos.y;
		gravityTime = 0;
		gravityAcc = 0;
	}
	gravityPos.y += gravityAcc;
	gravityAcc = -0.5f * 9.8f * powf(gravityTime / 60.0f, 2);
	if (gravityPos.y < -25)
	{
		gravityPos.y = -25;
	}
	gravityTime += 1.0f;
	gravity->SetPosition(gravityPos);
	
	//砲弾
	if (bulletPos.y == 0)
	{
		bulletPos.x = 0;
		bulletPos.y = 0;
		bulletTimeX = 0;
		bulletTimeY = 0;
		bulletAcc = { 2, 1, 0 };
	}
	bulletPos.x += bulletAcc.x;
	bulletPos.y += bulletAcc.y;
	bulletAcc.x = bulletAcc.x - 0.5f * 1.0f * powf(bulletTimeX / 60.0f, 2);
	if (bulletAcc.x < 0)
	{
		bulletAcc.x = 0;
	}
	bulletAcc.y =  bulletAcc.y - 0.5f * 9.8f * powf(bulletTimeY / 60.0f, 2);
	if (bulletPos.y < 0)
	{
		bulletPos.y = 0;
	}
	bulletTimeX += 1.0f;
	bulletTimeY += 1.0f;
	bullet->SetPosition(bulletPos);
}

void GamePlayScene::Draw()
{
	//カメラとライトの更新
	light->Update();
	camera->Update();

	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//各描画
	DrawBackSprite(cmdList);
	DrawObject(cmdList);
	//DrawParticle(cmdList);
	//DrawUI(cmdList);
	DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	//前景スプライト描画
	Sprite::PreDraw(cmdList);

	demo_back->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::DrawObject(ID3D12GraphicsCommandList* cmdList)
{
	//オブジェクト描画
	Object3d::PreDraw(cmdList);

	gravity->Draw();
	bullet->Draw();

	Object3d::PostDraw();

	//スプライト描画
	Sprite::PreDraw(cmdList);

	Sprite::PostDraw();
}

void GamePlayScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	//UI描画
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void GamePlayScene::DrawParticle(ID3D12GraphicsCommandList* cmdList)
{
	//パーティクル描画
	particle->Draw(cmdList);
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	//デバッグテキスト描画
	debugText.Draw(cmdList);
}