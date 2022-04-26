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
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	camera = Camera::GetInstance();
	
	//スプライトテクスチャ読み込み
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/background.png");

	//FBXの読み込み
	FbxLoader::GetInstance()->LoadModelFromFile("cube");

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

	isGravity = false;
	gravityTime = 0;
	gravityAcc = 0;
	isBullet = false;
	bulletTimeX = 0;
	bulletTimeY = 0;
	bulletAcc = { 2, 1, 0 };

	camera->SetEye({ 0, 0, -50 });
	camera->MoveCamera({0, 0, 0});

	gravity->SetPosition({ -15, 25, 0 });
	gravity->Update();

	bullet->SetPosition({ 0, 0, 0 });
	bullet->Update();
}

void GamePlayScene::Update()
{
	//座標取得
	XMFLOAT3 gravityPos = gravity->GetPosition();
	XMFLOAT3 bulletPos = bullet->GetPosition();

	//スイッチ
	if (input->TriggerKey(DIK_R) && isGravity == false && isBullet == false)
	{
		isGravity = true;
		gravityPos.y = 25;
		gravityTime = 0;
		gravityAcc = 0;
		isBullet = true;
		bulletPos.x = 0;
		bulletPos.y = 0;
		bulletTimeX = 0;
		bulletTimeY = 0;
		bulletAcc = { 2, 1, 0 };
	}

	//自由落下
	if (isGravity == true)
	{
		//加算
		gravityPos.y += gravityAcc;
		//加速度
		gravityAcc = -0.5f * 9.8f * powf(gravityTime / 60.0f, 2);

		//座標をセット
		if (gravityPos.y < -25)
		{
			gravityPos.y = -25;
			isGravity = false;
		}
		gravity->SetPosition(gravityPos);

		//時間
		gravityTime += 1.0f;
	}
	
	//砲弾
	if (isBullet == true)
	{
		//加算
		bulletPos.x += bulletAcc.x;
		bulletPos.y += bulletAcc.y;
		//加速度
		bulletAcc.x = bulletAcc.x - 0.5f * 1.0f * powf(bulletTimeX / 60.0f, 2);

		//座標をセット
		if (bulletAcc.x < 0)
		{
			bulletAcc.x = 0;
		}
		bulletAcc.y = bulletAcc.y - 0.5f * 9.8f * powf(bulletTimeY / 60.0f, 2);
		if (bulletPos.y < 0)
		{
			bulletPos.y = 0;
			isBullet = false;
		}
		bullet->SetPosition(bulletPos);

		//時間
		bulletTimeX += 1.0f;
		bulletTimeY += 1.0f;
	}

	debugText.Print("R : Start / ReStart", 10, 10, 2);
	debugText.Print("Gravity", 250, 200, 2);
	debugText.Print("Bullet", 600, 200, 2);
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