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

	//ライト生成
	light.reset(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightDir({-5, -5, 0, 0});
	Object3d::SetLight(light.get());

	//前景スプライト
	debugText.Initialize(fontNumber);

	//パーティクル
	particle.reset(ParticleManager::Create());

	//スプライト
	demo_back.reset(Sprite::CreateSprite(1));

	//OBJオブジェクト
	for (int i = 0; i < defaultWall.size(); i++)
	{
		defaultWall[i].reset(Object3d::Create("Wall"));
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
	fbxObject->SetRotation({ 0, 90, 0 });
	fbxObject->SetScale({ 1, 1, 1 });
	fbxObject->Update();

	for (int i = 0; i < defaultWall.size(); i++)
	{
		float size = 100;
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
	if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		vec.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.5f;
	}
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		vec.z += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 0.5f;
	}
	XMFLOAT3 pos = camera->ConvertWindowPos(fbxObject->GetPosition(), vec);
	fbxObject->SetPosition(pos);

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
	camera->FollowUpCamera(pos, camera->GetDistance(), angle.x, angle.y);
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
	DrawObjects(cmdList);
	DrawEffect(cmdList);
	DrawUI(cmdList);
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

void GamePlayScene::DrawObjects(ID3D12GraphicsCommandList* cmdList)
{
	//OBJオブジェクト描画
	Object3d::PreDraw(cmdList);

	for (int i = 0; i < defaultWall.size(); i++)
	{
		//defaultWall[i]->Draw();
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

void GamePlayScene::DrawEffect(ID3D12GraphicsCommandList* cmdList)
{
	//パーティクル描画
	//particle->Draw(cmdList);
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	//デバッグテキスト描画
	debugText.Draw(cmdList);
}
