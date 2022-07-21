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
	m_bullet.clear();
	m_bullet.shrink_to_fit();
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
	//particle.reset(ParticleManager::Create());

	//スプライト
	demo_back.reset(Sprite::CreateSprite(1));

	//OBJオブジェクト
	obj.reset(Object3d::Create("Dragon", true));
	for (auto& m : defaultWall)
	{
		m.reset(Object3d::Create("Wall"));
	}
	enemy.reset(Object3d::Create("Enemy"));

	//FBXオブェクト
	//fbxObject.reset(FbxObject3d::CreateFBXObject("Human"));
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

	bulletTime = 0;

	/*float fbxObjectSize = 0.5f;
	fbxObject->SetPosition({ 0, 0, -100 });
	fbxObject->SetRotation({ -90, 0, 0 });
	fbxObject->SetScale({ fbxObjectSize, fbxObjectSize, fbxObjectSize });
	fbxObject->Update()*/

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
		vec.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.5f;
		vec.z += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 0.5f;
	}
	XMFLOAT3 pos = camera->ConvertWindowPos(obj->GetPosition(), vec);
	obj->SetPosition(pos);
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

	//敵の攻撃
	bulletTime++;
	if (bulletTime > 30)
	{
		bulletTime = 0;
		
		bool active = false;
		if (m_bullet.size() > 0)
		{
			for (auto& m : m_bullet)
			{
				if (m->GetAlive() == false)
				{
					m->activeBullet(enemy->GetPosition(), XMFLOAT3(0, 0, -1));
					active = true;
					break;
				}
			}
		}
		if (active == false)
		{
			m_bullet.emplace_back(new Bullet(enemy->GetPosition(), XMFLOAT3(0, 0, -1)));
		}
	}
	for (auto& m : m_bullet)
	{
		m->Update();
	}

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
	//fbxObject->SetPosition(pos);
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
	for (auto& m : defaultWall)
	{
		m->Draw(cmdList);
	}
	enemy->Draw(cmdList);
	obj->Draw(cmdList);
	for (auto& m : m_bullet)
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
