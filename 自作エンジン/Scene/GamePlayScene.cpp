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
	ball.reset(Object3d::Create("Bullet", true));
	p.reset(Object3d::Create("ball", true));

	//パラメーター
	camera->SetEye({ 0, 0, -300 });
	ResetVariable();

	//オーディオ
	audio->Initialize();
}

void GamePlayScene::ResetVariable()
{
	isStart = false;

	r = 200;

	vt = 0;

	t = 0;

	p->SetPosition({0, 0, 0});
	ball->SetPosition({ r, 0, 0 });

	p->SetScale({ 10, 10, 10 });
	ball->SetScale({ 10, 10, 10 });

	p->Update();
	ball->Update();
}

void GamePlayScene::Update()
{
	if (input->TriggerKey(DIK_R) && isStart == false)
	{
		ResetVariable();
		isStart = true;
	}
	if (input->PushKey(DIK_S) && isStart == true)
	{
		isStart = false;
	}

	//座標取得
	if (isStart)
	{
		XMFLOAT3 posP = p->GetPosition();
		XMFLOAT3 posBall = ball->GetPosition();

		


		p->SetPosition(posP);
		ball->SetPosition(posBall);

		t++;
	}

	debugText.Print("R : Start / ReStart", 10, 10, 2);
	debugText.Print("S : Stop", 10, 50, 2);
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

	p->Draw();
	ball->Draw();

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