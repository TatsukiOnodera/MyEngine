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
	//audio = Audio::GetInstance();;
	camera = Camera::GetInstance();

	// サウンドファイル読み込み
	//audio->LoadSound("Resources");

	// スプライトテクスチャ読み込み
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");


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


	// OBJオブジェクト
	for (auto& m : wall)
	{
		m.reset(Object3d::Create("Wall"));
	}
	for (int e = 0; e < 6; e++)
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
	float size = 200;
	for (int i = 0; i < wall.size(); i++)
	{
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

		wall[i]->SetPosition(pos);
		wall[i]->SetRotation(rot);
		wall[i]->SetScale(scale);
		wall[i]->Update();
	}

	camera->SetTarget({ 0, 0, 0 });
	camera->SetDistance();
	camera->Update();
}

void GamePlayScene::Update()
{
#pragma region ゲームメインシステム

	// プレイヤー
	player->Update();
	
	// エネミー
	for (const auto& m : enemy)
	{
		m->Update();
	}
	
	// 衝突判定
	CheckAllCollisions();

#pragma endregion

#pragma region カメラとライトの更新

	// ライト更新
	light->Update();
	
	// カメラ更新
	camera->Update();

#pragma endregion
}

void GamePlayScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	// 各描画
	//DrawBackSprite(cmdList);
	DrawObjects(cmdList);
	DrawEffect(cmdList);
	DrawUI(cmdList);
	//DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	// 前景スプライト描画
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::DrawObjects(ID3D12GraphicsCommandList* cmdList)
{
	// OBJオブジェクト描画
	Object3d::PreDraw(cmdList);

	// 壁
	for (auto& m : wall)
	{
		m->Draw();
	}

	Object3d::PostDraw();

	// FBXオブジェクト
	FbxObject3d::PreDraw(cmdList);



	FbxObject3d::PostDraw();

	// プレイヤー
	player->Draw(cmdList);

	// エネミー
	for (const auto& m : enemy)
	{
		m->Draw(cmdList);
	}

	// スプライト描画
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
}

void GamePlayScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	// UI描画
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void GamePlayScene::DrawEffect(ID3D12GraphicsCommandList* cmdList)
{
	// パーティクル描画
	ParticleManager::PreDraw(cmdList);

	particle->Draw();

	ParticleManager::PostDraw();
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	// デバッグテキスト描画
	debugText.Draw(cmdList);
}

const float GamePlayScene::Length(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	XMFLOAT3 len = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}

void GamePlayScene::CheckAllCollisions()
{
	// プレイヤーとエネミーの座標
	XMFLOAT3 playerPos = player->GetPosition();
	XMFLOAT3 enemyPos[6] = {};
	for (int e = 0; e < enemy.size(); e++)
	{
		enemyPos[e] = enemy[e]->GetPosition();
	}

#pragma region プレイヤーバレットとエネミーの当たり判定

	

#pragma endregion

#pragma region プレイヤーとエネミーバレットの当たり判定

	

#pragma endregion

#pragma region プレイヤーとエネミーの当たり判定

	for (int e = 0; e < enemy.size(); e++)
	{
		if (player->GetAlive() == true && enemy[e]->GetAlive() == true && Length(playerPos, enemyPos[e]) < 20.0f)
		{
			/*XMFLOAT3 vel = enemy[e]->GetVelocity();
			vel.x = -vel.x;
			vel.y = -vel.y;
			vel.z = -vel.z;
			enemy[e]->SetVelocity(vel);*/
		}
	}

#pragma endregion

#pragma region プレイヤーと壁当たり判定

	if (player->GetAlive() == true)
	{
		if (wall[FRONT]->GetPosition().z < playerPos.z)
		{
			playerPos.z = wall[FRONT]->GetPosition().z;
			player->OnCollision();
		}
		else if (playerPos.z < wall[BACK]->GetPosition().z)
		{
			playerPos.z = wall[BACK]->GetPosition().z;
			player->OnCollision();
		}
		if (wall[RIGHT]->GetPosition().x < playerPos.x)
		{
			playerPos.x = wall[RIGHT]->GetPosition().x;
			player->OnCollision();
		}
		else if (playerPos.x < wall[LEFT]->GetPosition().x)
		{
			playerPos.x = wall[LEFT]->GetPosition().x;
			player->OnCollision();
		}
		if (wall[UP]->GetPosition().y < playerPos.y)
		{
			playerPos.y = wall[UP]->GetPosition().y;
			player->OnCollision();
		}
		else if (playerPos.y < wall[DOWN]->GetPosition().y + 1.875f)
		{
			playerPos.y = wall[DOWN]->GetPosition().y + 1.875f;
			player->OnCollision();
		}
		player->SetPosition(playerPos);
	}

#pragma endregion

#pragma region エネミーと壁当たり判定

	for (int e = 0; e < enemy.size(); e++)
	{
		if (enemy[e]->GetAlive() == true)
		{
			if (wall[FRONT]->GetPosition().z < enemyPos[e].z)
			{
				enemyPos[e].z = wall[FRONT]->GetPosition().z;
				enemy[e]->OnCollision();
			}
			else if (enemyPos[e].z < wall[BACK]->GetPosition().z)
			{
				enemyPos[e].z = wall[BACK]->GetPosition().z;
				enemy[e]->OnCollision();
			}
			if (wall[RIGHT]->GetPosition().x < enemyPos[e].x)
			{
				enemyPos[e].x = wall[RIGHT]->GetPosition().x;
				enemy[e]->OnCollision();
			}
			else if (enemyPos[e].x < wall[LEFT]->GetPosition().x)
			{
				enemyPos[e].x = wall[LEFT]->GetPosition().x;
				enemy[e]->OnCollision();
			}
			if (wall[UP]->GetPosition().y < enemyPos[e].y)
			{
				enemyPos[e].y = wall[UP]->GetPosition().y;
				enemy[e]->OnCollision();
			}
			else if (enemyPos[e].y < wall[DOWN]->GetPosition().y)
			{
				enemyPos[e].y = wall[DOWN]->GetPosition().y;
				enemy[e]->OnCollision();
			}
			enemy[e]->SetPosition(enemyPos[e]);
		}
	}

#pragma endregion
}
