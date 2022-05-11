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

	//前景スプライト
	debugText.Initialize(fontNumber);

	//ライト生成
	light.reset(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightDir({-1, -1, 0, 0});
	Object3d::SetLight(light.get());

	//スプライト
	demo_back.reset(Sprite::CreateSprite(1));

	//オブジェクト
	airResistance.reset(Object3d::Create("Bullet", true));
	friction.reset(Object3d::Create("Bullet", true));

	//パラメーター
	camera->SetEye({ 0, 0, -500 });
	ResetVariable();

	//オーディオ
	audio->Initialize();
}

void GamePlayScene::ResetVariable()
{
	timer = 0;

	is_airResistance = false;
	is_friction = false;

	airAcc = { 0, 0 };
	friAcc = 0;

	kv = 0;

	airV = { 20, 8 };
	friV = 10;

	airResistance->SetPosition({-400, 0, 0});
	friction->SetPosition({ -400, -250, 0 });

	airResistance->SetScale({ 10, 10, 10 });
	friction->SetScale({ 10, 10, 10 });

	airResistance->Update();
	friction->Update();
}

void GamePlayScene::Update()
{
	if (input->TriggerKey(DIK_R) && is_airResistance == false && is_friction == false)
	{
		ResetVariable();
		is_airResistance = true;
		is_friction = true;
	}

	//座標取得
	XMFLOAT3 airPos = airResistance->GetPosition();
	XMFLOAT3 friPos = friction->GetPosition();

	if (is_airResistance == true)
	{
		//重力
		airAcc.y = -0.5f * gravity * powf(static_cast<float>(timer) / 60, 2);
		airV.y += airAcc.y;
		airPos.y += airV.y;
		if (airPos.y < 0)
		{
			airPos.y = 0;
			is_airResistance = false;
		}

		//横に飛ぶ（空気抵抗）
		kv = 0.001f * airV.x;
		airAcc.x = kv / 0.1f;
		airV.x -= airAcc.x;
		if (airV.x < 0)
		{
			airV.x = 0;
		}
		airPos.x += airV.x;

		airResistance->SetPosition(airPos);
	}
	if (is_friction == true)
	{
		//摩擦
		float m = 20.0f;
		float N = m * (gravity * powf(static_cast<float>(timer) / 60, 2));
		float F = friV - (0.8 * N);
		friAcc = F / m;
		friV += friAcc;
		if (friV < 0)
		{
			friV = 0;
			is_friction = false;
		}
		friPos.x += friV;

		friction->SetPosition(friPos);
	}
	if (is_airResistance == true || is_friction == true)
	{
		timer++;
	}

	debugText.Print("R : Start / ReStart", 10, 10, 2);
	debugText.Print("airResistance", 600, 50, 2);
	debugText.Print("friction", 600, 400, 2);
}

void GamePlayScene::Draw()
{
	//カメラとライトの更新
	light->Update();
	camera->Update();

	//コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//各描画
	DrawBackSprite(cmdList);
	Draw(cmdList);
	//DrawParticle(cmdList);
	//DrawUI(cmdList);
	DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	//前景スプライト描画
	Sprite::PreDraw(cmdList);

	//demo_back->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//OBJオブジェクト描画
	Object3d::PreDraw(cmdList);

	airResistance->Draw();
	friction->Draw();

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