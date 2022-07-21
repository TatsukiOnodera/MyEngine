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
	obj.reset(Object3d::Create("Dragon", true));
	for (auto& m : defaultWall)
	{
		m.reset(Object3d::Create("Wall"));
	}
	enemy.reset(Object3d::Create("Enemy"));
	bullet.emplace_back(Object3d::Create("Bullet"));

	//FBXオブェクト
	fbxObject.reset(FbxObject3d::CreateFBXObject("Human"));

	//パーティクル
	//particle.reset(ParticleManager::Create());
	//fbxObject.reset(FbxObject3d::CreateFBXObject("boneTest"));
	//fbxObject->PlayAnimation(true);

	//パラメーター
	ResetParameter();

	//オーディオ
	audio->Initialize();
}

void GamePlayScene::ResetParameter()
{
	isDash = false;
	add0 = 0;
	bulletVec.emplace_back(XMFLOAT3(0, 0, 0));
	bulletAlive.emplace_back(false);
	bulletTime = 0;

	/*float fbxObjectSize = 0.5f;
	fbxObject->SetPosition({ 0, 0, -100 });
	fbxObject->SetRotation({ -90, 0, 0 });
	fbxObject->SetScale({ fbxObjectSize, fbxObjectSize, fbxObjectSize });
	fbxObject->Update();*/
	//fbxObject->SetRotation({ 0, 90, 0 });
	//fbxObject->Update();

	obj->SetPosition({ 0, 0, 0 });
	obj->SetScale({ 3, 3, 3 });
	obj->SetColor({ 0, 0.6, 0.2, 1 });
	obj->SetShader(SPECULAR);
	obj->SetMaskTexture("Resources/", "Scales.png");
	obj->Update();

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

	enemy->SetScale({1.5, 1.5, 1.5});
	enemy->Update();

	for (auto& m : bullet)
	{
		m->SetPosition(enemy->GetPosition());
		m->SetScale({0.5, 0.5, 0.5});
		m->Update();
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
	//if (input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
	//{
	//	//ダッシュ
	//	if (input->TriggerKey(DIK_SPACE))
	//	{
	//		isDash = true;
	//		add0 = 25;

	//		//アニメーション
	//		fbxObject->PlayAnimation(false);

	//		//パーティクル
	//		//particle->Active(fbxObject->GetPosition());
	//	}

	//	//ベクトル
	//	vec.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.5f;
	//	vec.z += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 0.5f;
	//	//加速するなら
	//	if (isDash)
	//	{
	//		vec.x *= add0;
	//		vec.z *= add0;

	//		add0 = add0 - 10;

	//		//加速度が0になったら
	//		if (add0 <= 0)
	//		{
	//			add0 = 0;
	//			isDash = false;
	//		}
	//	}
	//}
	//else
	//{
	//	//アニメーション
	//	fbxObject->ResetAnimation();
	//}
	//カメラを軸にした変換
	/*XMFLOAT3 pos = fbxObject->GetPosition();
	pos = camera->ConvertWindowPos(pos, vec);*/

	////敵の攻撃
	//bulletTime++;
	//if (bulletTime > 30)
	//{
	//	bulletTime = 0;
	//	bool hit = false;
	//	for (int i = 0; i < bulletAlive.size(); i++)
	//	{
	//		if (bulletAlive[i] == false)
	//		{
	//			hit = true;
	//			bulletAlive[i] = true;
	//			if (bullet.size() <= i && bulletVec.size() <= i)
	//			{
	//				bullet.emplace_back(Object3d::Create("Bullet"));
	//				bulletVec.emplace_back(XMFLOAT3(0, 0, 0));
	//				assert(bullet.size() <= i && bulletVec.size() <= i);
	//			}
	//			XMFLOAT3 bVec;
	//			bVec.x = (pos.x - enemy->GetPosition().x) / 25;
	//			bVec.y = (pos.y - enemy->GetPosition().y) / 25;
	//			bVec.z = (pos.z - enemy->GetPosition().z) / 25;
	//			bulletVec[i] = bVec;
	//		}
	//	}
	//	if (hit == false)
	//	{
	//		bulletAlive.emplace_back(true);
	//		int i = bulletAlive.size() - 1;
	//		if (bullet.size() <= i && bulletVec.size() <= i)
	//		{
	//			bullet.emplace_back(Object3d::Create("Bullet"));
	//			bulletVec.emplace_back(XMFLOAT3(0, 0, 0));
	//			assert(i < bullet.size()&& i < bulletVec.size());
	//		}
	//		XMFLOAT3 bVec;
	//		bVec.x = (pos.x - enemy->GetPosition().x) / 25;
	//		bVec.y = (pos.y - enemy->GetPosition().y) / 25;
	//		bVec.z = (pos.z - enemy->GetPosition().z) / 25;
	//		bulletVec[i] = bVec;
	//	}
	//
	//for (int i = 0; i < bulletAlive.size(); i++)
	//{
	//	if (bulletAlive[i])
	//	{
	//		XMFLOAT3 bPos = bullet[i]->GetPosition();
	//		bPos.x += bulletVec[i].x;
	//		bPos.y += bulletVec[i].y;
	//		bPos.z += bulletVec[i].z;
	//		bullet[i]->SetPosition(bPos);
	//		if (bPos.x < -300 || 300 < bPos.x)
	//		{
	//			bulletAlive[i] = false;
	//			bulletVec[i] = { 0, 0, 0 };
	//			bullet[i]->SetPosition({ 0, 0, 0 });
	//		}
	//		else if (bPos.z < -300 || 300 < bPos.z)
	//		{
	//			bulletAlive[i] = false;
	//			bulletVec[i] = { 0, 0, 0 };
	//			bullet[i]->SetPosition({ 0, 0, 0 });
	//		}
	//	}
	//}
	//	vec.z += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 0.5f;
	//}

	if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		vec.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.5f;
	}
	XMFLOAT3 pos = camera->ConvertWindowPos(obj->GetPosition(), vec);
	obj->SetPosition(pos);

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

	demo_back->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::DrawObjects(ID3D12GraphicsCommandList* cmdList)
{
	//OBJオブジェクト描画
	for (const auto& m : defaultWall)
	{
		m->Draw(cmdList);
	}
	enemy->Draw(cmdList);
	for (int i = 0; i < bulletAlive.size(); i++)
	{
		if (bulletAlive[i])
		{
			bullet[i]->Draw(cmdList);
		}
	}
	obj->Draw(cmdList);
	for (auto& m : defaultWall)
	{
		m->Draw(cmdList);
	}

	//FBXオブジェクト
	FbxObject3d::PreDraw(cmdList);

	//fbxObject->Draw();

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
