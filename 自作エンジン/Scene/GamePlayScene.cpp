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
	

	//オブジェクト
	ballA.reset(Object3d::Create("Bullet", true));
	ballB.reset(Object3d::Create("chr_sword", true));

	//パラメーター
	camera->SetEye({ 0, 0, -500 });
	ResetVariable();

	//オーディオ
	audio->Initialize();
}

void GamePlayScene::ResetVariable()
{
	timer = 0;

	isStart = false;

	v0A = 10;
	v0B = -10;

	vA = v0A;
	vB = v0B;

	mA = 20;
	mB = 10;

	rA = 10;
	rB = 10;

	ballA->SetPosition({-400, -100, 0});
	ballB->SetPosition({ 400, -100, 0 });

	ballA->SetScale({ rA, rA, rA });
	ballB->SetScale({ rB, rB, rB });

	ballA->Update();
	ballB->Update();
}

void GamePlayScene::Update()
{
	if (input->TriggerKey(DIK_R))
	{
		ResetVariable();
		isStart = true;
	}

	//座標取得
	XMFLOAT3 posA = ballA->GetPosition();
	XMFLOAT3 posB = ballB->GetPosition();

	if (isStart == true)
	{
		posA.x += vA;
		posB.x += vB;

		ballA->SetPosition(posA);
		ballB->SetPosition(posB);
	}
	if (powf(posA.x - posB.x, 2) <= powf(rA + rB, 2))
	{
		//左辺
		float left = mA * vA + mB * vB;
		vA = left / (mA * -1 + mA * 0);
		vB = left / (mB * 0 + mB * 1);
	}
	if (isStart == true)
	{
		timer++;
		if (timer > 60)
		{
			isStart = false;
		}
	}

	debugText.Print("R : Start / ReStart", 10, 10, 2);
}

void GamePlayScene::Draw()
{
	//カメラとライトの更新
	light->Update();
	camera->Update();

	//コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//各描画
	//DrawBackSprite(cmdList);
	Draw(cmdList);
	//DrawParticle(cmdList);
	//DrawUI(cmdList);
	DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	//前景スプライト描画
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//OBJオブジェクト描画
	Object3d::PreDraw(cmdList);

	ballA->Draw();
	ballB->Draw();

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