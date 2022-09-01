#include "GamePlayScene.h"
#include "SceneManager.h"

#include <SafeDelete.h>
#include <cassert>
#include <fbxsdk.h>

using namespace DirectX;

GamePlayScene::~GamePlayScene()
{
	
}

void GamePlayScene::Initialize()
{
	dx_cmd = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	//audio = Audio::GetInstance();
	camera = Camera::GetInstance();

	// スプライトテクスチャ読み込み
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/background.png");
	Sprite::LoadTexture(2, L"Resources/Reticle.png");

<<<<<<< HEAD
	// ライト生成
	light.reset(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightDir({ -5, -5, 0, 0 });
	Object3d::SetLight(light.get());

	// 前景スプライト
	debugText.Initialize(fontNumber);

	// パーティクル
	particle.reset(ParticleManager::Create("Default/effect1.png"));

	// スプライト
	sight.reset(Sprite::Create(2, { 0.0f, 0.0f }, { 0.5f, 0.5f }));

	// OBJオブジェクト
	for (auto& m : defaultWall)
	{
		m.reset(Object3d::Create("Wall"));
	}
	for (int i = 0; i < 6; i++)
	{
		enemy.emplace_back(new Enemy);
	}

	// FBXオブェクト
	player.reset(new Player);

	// オーディオ
	//audio->Initialize();

	// 変数の初期化
	InitializeVariable();

	// 乱数初期化
	srand(NULL);
}

void GamePlayScene::InitializeVariable()
{
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
	camera->SetEye({ 0, 1, -5 });
	camera->SetDistance();
	camera->InitializeAngle();
	camera->Update();

	targetNum = 0;
	listNum = 0;
}

void GamePlayScene::Update()
{
#pragma region ゲームメインシステム
	// プレイヤー
	player->Update();
	targetList.clear();
	for (int i = 0; i < enemy.size(); i++)
	{
		if (Length(enemy[i]->GetPosition(), player->GetPosition()) < 90 && enemy[i]->GetAlive())
		{
			targetList.emplace_back(i);
		}
	}
	if (input->TriggerKey(DIK_P) || input->TriggerKey(DIK_O))
	{
		listNum += input->TriggerKey(DIK_P) - input->TriggerKey(DIK_O);
	}
	if (listNum >= targetList.size())
	{
		listNum = 0;
	}
	if (targetList.size() > 0)
	{
		targetNum = targetList[listNum];
	}
	player->ShotBullet(enemy[targetNum]->GetPosition());
	for (auto& m : enemy)
	{
		if (player->bulletUpdate(m->GetPosition()))
		{
			m->SetEffectTimer();
		}
	}
	
	// エネミー
	for (auto& m : enemy)
	{
		if (m->Update(player->GetPosition()))
		{
			player->SetEffectTimer();
		}
	}

	bool isEnd = true;
	for (auto& m : enemy)
	{
		if (m->GetAlive())
		{
			isEnd = false;
=======
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
	//fbxObject->SetRotation({ 0, 90, 0 });
	//fbxObject->Update();

	obj->SetPosition({ 0, 0, 0 });
	obj->SetScale({ 1, 1, 1 });
	obj->Update();

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
>>>>>>> parent of 00f20c5... no message
		}
		defaultWall[i]->SetPosition(pos);
		defaultWall[i]->SetRotation(rot);
		defaultWall[i]->SetScale(scale);
		defaultWall[i]->Update();
	}
<<<<<<< HEAD
	if (isEnd)
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("END");
=======

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
>>>>>>> parent of 00f20c5... no message
	}
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		vec.z += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 0.5f;
	}
	XMFLOAT3 pos = camera->ConvertWindowPos(fbxObject->GetPosition(), vec);
	fbxObject->SetPosition(pos);

<<<<<<< HEAD
	//サイト
	sight->SetPosition(camera->Convert3DPosTo2DPos(enemy[targetNum]->GetPosition()));

	// カメラ
=======
	//カメラ
>>>>>>> parent of 00f20c5... no message
	XMFLOAT2 angle = { 0, 0 };
	if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		angle.y += (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * 1;
	}
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		angle.x += (input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN)) * 1;
	}
<<<<<<< HEAD
	// 追従カメラ
	camera->FollowUpCamera(player->GetPosition(), camera->GetDistance(), angle.x, angle.y);

#pragma endregion

#pragma region カメラとライトの更新
=======
	camera->FollowUpCamera(pos, camera->GetDistance(), angle.x, angle.y);
}
>>>>>>> parent of 00f20c5... no message

	light->Update();
	camera->Update();

#pragma endregion
}

void GamePlayScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

<<<<<<< HEAD
	// 各描画
	//DrawBackSprite(cmdList);
=======
	//各描画
	DrawBackSprite(cmdList);
>>>>>>> parent of 00f20c5... no message
	DrawObjects(cmdList);
	DrawEffect(cmdList);
	DrawUI(cmdList);
	DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	// 前景スプライト描画
	Sprite::PreDraw(cmdList);

	//demo_back->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::DrawObjects(ID3D12GraphicsCommandList* cmdList)
{
	// OBJオブジェクト描画
	Object3d::PreDraw(cmdList);

<<<<<<< HEAD
	// 壁
	for (auto& m : defaultWall)
	{
		m->Draw();
	}

	// 敵
	for (auto& m : enemy)
	{
		m->Draw();
=======
	for (int i = 0; i < defaultWall.size(); i++)
	{
		//defaultWall[i]->Draw();
>>>>>>> parent of 00f20c5... no message
	}

	Object3d::PostDraw();

<<<<<<< HEAD
	// FBXオブジェクト
	FbxObject3d::PreDraw(cmdList);

	// 自機
	player->Draw();

	FbxObject3d::PostDraw();

	// スプライト描画
=======
	//FBXオブジェクト
	FbxObject3d::PreDraw(cmdList);

	fbxObject->Draw();

	FbxObject3d::PostDraw();

	//スプライト描画
>>>>>>> parent of 00f20c5... no message
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
}

void GamePlayScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	// UI描画
	Sprite::PreDraw(cmdList);

	//サイト
	if (Length(enemy[targetNum]->GetPosition(), player->GetPosition()) < 90 && enemy[targetNum]->GetAlive())
	{
		sight->Draw();
	}

	Sprite::PostDraw();
}

void GamePlayScene::DrawEffect(ID3D12GraphicsCommandList* cmdList)
{
<<<<<<< HEAD
	// パーティクル描画
	ParticleManager::PreDraw(cmdList);

	particle->Draw();

	ParticleManager::PostDraw();
=======
	//パーティクル描画
	//particle->Draw(cmdList);
>>>>>>> parent of 00f20c5... no message
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	// デバッグテキスト描画
	debugText.Draw(cmdList);
}
<<<<<<< HEAD

const float GamePlayScene::Length(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	XMFLOAT3 len = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}

//for (int i = 0; i < 30; i++)
	//{
	//	const float rnd_pos = 10.0f;
	//	XMFLOAT3 pos = {};
	//	pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//	pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//	pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

	//	const float rnd_vel = 1.0f;
	//	XMFLOAT3 vel = {};
	//	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//	vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	//	XMFLOAT3 acc = {};
	//	//追加
	//	particle->Add(120, pos, vel, acc, 5.0f, 0.0f);
	//}
=======
>>>>>>> parent of 00f20c5... no message
