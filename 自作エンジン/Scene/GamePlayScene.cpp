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

	//OBJオブジェクト
	/*chr.reset(Object3d::Create("chr_sword"));
	player.reset(Object3d::Create("bullet", true));*/

	//FBXオブェクト
	fbxObject.reset(FbxObject3d::CreateFBXObject("boneTest"));

	//パラメーター
	ResetVariable();

	//オーディオ
	audio->Initialize();
}

void GamePlayScene::ResetVariable()
{
	/*angleX = 0;
	angleY = 0;

	chr->SetPosition({ 0, 0, 10 });
	chr->Update();

	player->SetPosition({ 0, 0, 0 });
	player->Update();*/

	fbxObject->SetPosition({ 0, 0, 0 });
	fbxObject->SetScale({ 1, 1, 1 });
	fbxObject->Update();

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 5, -10 });
	camera->Update();
}

void GamePlayScene::Update()
{
	//{
	//	//自機移動
	//	XMFLOAT3 pos = player->GetPosition();
	//	XMFLOAT3 vec = { 0, 0, 0 };
	//	if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
	//	{
	//		vec.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.25f;
	//	}
	//	if (input->PushKey(DIK_W) || input->PushKey(DIK_S))
	//	{
	//		vec.z += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 0.25f;
	//	}
	//	pos = camera->ConvertWindowPos(pos, vec, angleY);
	//	player->SetPosition(pos);
	//
	//	//カメラ回転
	//	if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	//	{
	//		angleY += (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * 2;
	//	}
	//	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	//	{
	//		angleX += (input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN)) * 2;
	//	}
	//	XMFLOAT3 eye = camera->FollowUpCamera(pos, { 0, 1, -10 }, angleX, angleY);
	//	camera->SetEye(eye);
	//
	//	//リセット
	//	if (input->TriggerKey(DIK_SPACE))
	//	{
	//		ResetVariable();
	//	}
	//}

	//移動
	XMFLOAT3 pos = fbxObject->GetPosition();
	if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		pos.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.25f;
	}
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		pos.y += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 0.25f;
	}
	fbxObject->SetPosition(pos);
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
	AnyDraw(cmdList);
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

void GamePlayScene::AnyDraw(ID3D12GraphicsCommandList* cmdList)
{
	//OBJオブジェクト描画
	Object3d::PreDraw(cmdList);

	/*chr->Draw();
	player->Draw();*/

	Object3d::PostDraw();

	//FBXオブジェクト
	FbxObject3d::PreDraw(cmdList);

	fbxObject->Draw();

	FbxObject3d::PostDraw();

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