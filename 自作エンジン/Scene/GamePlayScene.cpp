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
	light->SetLightDir({-5, -5, 0, 0});
	Object3d::SetLight(light.get());

	//スプライト
	demo_back.reset(Sprite::CreateSprite(1));

	//OBJオブジェクト
	for (int i = 0; i < wall.size(); i++)
	{
		wall[i].reset(Object3d::Create("Wall"));
	}

	//FBXオブェクト
	fbxObject.reset(FbxObject3d::CreateFBXObject("boneTest"));
	fbxObject->PlayAnimation(true);

	//パラメーター
	ResetVariable();

	//オーディオ
	audio->Initialize();
}

void GamePlayScene::ResetVariable()
{
	fbxObject->SetPosition({ 0, 0, 0 });
	fbxObject->Update();

	for (int i = 0; i < FIN; i++)
	{
		float size = 100;
		XMFLOAT3 pos = { 0, 0, 0 };
		XMFLOAT3 rot = { 0, 0, 0 };
		XMFLOAT3 scale = { size, size, size };
		if (i == FRONT)
		{
			pos = { 0, 0, size };
			rot = { 0, 180, 0 };
		} 
		else if (i == BACK)
		{
			pos = { 0, 0, -size };
			rot = { 0, 0, 0 };
		}
		else if (i == RIGHT)
		{
			pos = { size, 0, 0 };
			rot = { 0, -90, 0 };
		}
		else if (i == LEFT)
		{
			pos = { -size, 0, 0 };
			rot = { 0, 90, 0 };
		}
		else if (i == UP)
		{
			pos = { 0, size, 0 };
			rot = { 90, 0, 0 };
		}
		else if (i == DOWN)
		{
			pos = { 0, -size, 0 };
			rot = { -90, 0, 0 };
		}
		wall[i]->SetPosition(pos);
		wall[i]->SetRotation(rot);
		wall[i]->SetScale(scale);
		wall[i]->Update();
	}

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 5, -10 });
	camera->Update();
}

void GamePlayScene::Update()
{
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

	//カメラ
	XMFLOAT3 eye = {0, 0, 0};
	if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		eye.x += (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * 0.25f;
	}
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		eye.y += (input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN)) * 0.25f;
	}
	if (input->PushKey(DIK_RSHIFT) || input->PushKey(DIK_END))
	{
		eye.z += (input->PushKey(DIK_END) - input->PushKey(DIK_RSHIFT)) * 0.25f;
	}
	camera->MoveCamera(eye);
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
	DrawOthers(cmdList);
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

void GamePlayScene::DrawOthers(ID3D12GraphicsCommandList* cmdList)
{
	//OBJオブジェクト描画
	Object3d::PreDraw(cmdList);

	for (int i = 0; i < FIN; i++)
	{
		wall[i]->Draw();
	}

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
