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
	for (int i = 0; i < defaultWall.size(); i++)
	{
		defaultWall[i].reset(Object3d::Create("Wall"));
	}

	//FBXオブェクト
	fbxObject.reset(FbxObject3d::CreateFBXObject("Human"));

	//パーティクル
	//particle.reset(ParticleManager::Create());

	//パラメーター
	ResetParameter();

	//オーディオ
	audio->Initialize();
}

void GamePlayScene::ResetParameter()
{
	isDash = false;
	add0 = 0;

	fbxObject->SetPosition({ 0, 0, 0 });
	fbxObject->SetRotation({ -90, 0, 0 });
	fbxObject->SetScale({ 0.25, 0.25, 0.25 });
	fbxObject->Update();

	for (int i = 0; i < defaultWall.size(); i++)
	{
		float size = 300;
		XMFLOAT3 pos;
		XMFLOAT3 rot;
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
		defaultWall[i]->SetPosition(pos);
		defaultWall[i]->SetRotation(rot);
		defaultWall[i]->SetScale(scale);
		defaultWall[i]->Update();
	}

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 5, -10 });
	camera->SetDistance();
	camera->Update();
}

void GamePlayScene::Update()
{
	//移動
	XMFLOAT3 vec = { 0, 0, 0 };
	if (input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		//ダッシュ
		if (input->TriggerKey(DIK_SPACE))
		{
			isDash = true;
			add0 = 50;

			//アニメーション
			fbxObject->PlayAnimation(false);

			//パーティクル
			//particle->Active(fbxObject->GetPosition());
		}

		//ベクトル
		vec.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.5f;
		vec.z += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 0.5f;
		//加速するなら
		if (isDash)
		{
			vec.x *= add0;
			vec.z *= add0;

			add0 = add0 - 5;

			//加速度が0になったら
			if (add0 <= 0)
			{
				add0 = 0;
				isDash = false;
			}
		}
	}
	else
	{
		//アニメーション
		fbxObject->ResetAnimation();
	}
	//カメラを軸にした変換
	XMFLOAT3 pos = fbxObject->GetPosition();
	pos = camera->ConvertWindowPos(pos, vec);

	//カメラ
	XMFLOAT2 angle = { 0, 0 };
	if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		angle.y += (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * 1;
	}
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		angle.x += (input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN)) * 1;
	}

	//座標をセット
	fbxObject->SetPosition(pos);
	//追従カメラ
	camera->FollowUpCamera(pos, camera->GetDistance(), angle.x, angle.y);

#pragma region カメラとライトの更新

	light->Update();
	camera->Update();

#pragma endregion
}

void GamePlayScene::Draw()
{
	//コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//各描画
	//DrawBackSprite(cmdList);
	DrawOthers(cmdList);
	//DrawParticle(cmdList);
	//DrawUI(cmdList);
	//DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	//前景スプライト描画
	Sprite::PreDraw(cmdList);

	demo_back->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::DrawOthers(ID3D12GraphicsCommandList* cmdList)
{
	//OBJオブジェクト描画
	Object3d::PreDraw(cmdList);

	for (const auto& m : defaultWall)
	{
		m->Draw();
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
