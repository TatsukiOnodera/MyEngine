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
	Sprite::LoadTexture(1, L"Resources/Reticle.png");

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
	reticle.reset(Sprite::Create(1, { 0, 0 }, { 0.5f, 0.5f }));

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

	reticle->Draw();

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
	XMFLOAT3 playerPos = player->GetPosition();
	XMFLOAT3 enemyPos[6] = {};
	for (int e = 0; e < enemy.size(); e++)
	{
		enemyPos[e] = enemy[e]->GetPosition();
	}
	const std::vector<std::unique_ptr<Bullet>>& playerBullets = player->GetPlayerBullets();

#pragma region プレイヤーとエネミーの当たり判定

	bool tmp_hit = false;
	for (int e = 0; e < enemy.size(); e++)
	{
		if (Length(playerPos, enemyPos[e]) < 80.0f && enemy[e]->GetAlive() == true)
		{
			targetNum = e + 1;
			tmp_hit = true;
		}
	}
	if (tmp_hit == false)
	{
		targetNum = 0;
	}
	if (targetNum == 0)
	{
		reticle->SetPosition({ 0, 0 });
		player->SetTargetPosition({0, 0, 0});
		player->SetIsLock(false);
		reticle->SetInvisible(true);
	}
	else
	{
		reticle->SetPosition(camera->Convert3DPosTo2DPos(enemyPos[targetNum - 1]));
		player->SetTargetPosition(enemyPos[targetNum - 1]);
		player->SetIsLock(true);
		reticle->SetInvisible(false);
	}

#pragma endregion

#pragma region プレイヤー弾とエネミーの当たり判定

	for (const auto& m : playerBullets)
	{
		for (int e = 0; e < enemy.size(); e++)
		{
			if (Length(m->GetPosition(), enemyPos[e]) < 5.0f)
			{
				m->OnCollision();
				enemy[e]->OnCollision();
			}
		}
	}

#pragma endregion

#pragma region プレイヤーとエネミー弾の当たり判定

	for (int e = 0; e < enemy.size(); e++)
	{
		const std::vector<std::unique_ptr<Bullet>>& enemyrBullets = enemy[e]->GetEnemyBullet();
		for (auto& m : enemyrBullets)
		{
			if (Length(m->GetPosition(), playerPos) < 10.0f)
			{
				m->OnCollision();
			}
		}
	}

#pragma endregion

#pragma region プレイヤーと壁当たり判定

	if (player->GetAlive() == true)
	{
		if (wall[FRONT]->GetPosition().z < playerPos.z)
		{
			playerPos.z = wall[FRONT]->GetPosition().z;
		}
		else if (playerPos.z < wall[BACK]->GetPosition().z)
		{
			playerPos.z = wall[BACK]->GetPosition().z;
		}
		if (wall[RIGHT]->GetPosition().x < playerPos.x)
		{
			playerPos.x = wall[RIGHT]->GetPosition().x;
		}
		else if (playerPos.x < wall[LEFT]->GetPosition().x)
		{
			playerPos.x = wall[LEFT]->GetPosition().x;
		}
		if (playerPos.y < wall[DOWN]->GetPosition().y + 1.875f)
		{
			playerPos.y = wall[DOWN]->GetPosition().y + 1.875f;
			player->isLanding();
		}
		player->SetPosition(playerPos);
	}

#pragma endregion

#pragma region エネミーと壁当たり判定

	for (int e = 0; e < enemy.size(); e++)
	{
		if (enemy[e]->GetAlive() == true)
		{
			XMFLOAT3 enemyVel = enemy[e]->GetVelocity();

			if (wall[FRONT]->GetPosition().z < enemyPos[e].z)
			{
				enemyPos[e].z = wall[FRONT]->GetPosition().z;
				enemyVel.z = -fabs(enemyVel.z);
			}
			else if (enemyPos[e].z < wall[BACK]->GetPosition().z)
			{
				enemyPos[e].z = wall[BACK]->GetPosition().z;
				enemyVel.z = fabs(enemyVel.z);
			}
			if (wall[RIGHT]->GetPosition().x < enemyPos[e].x)
			{
				enemyPos[e].x = wall[RIGHT]->GetPosition().x;
				enemyVel.x = -fabs(enemyVel.x);
			}
			else if (enemyPos[e].x < wall[LEFT]->GetPosition().x)
			{
				enemyPos[e].x = wall[LEFT]->GetPosition().x;
				enemyVel.x = fabs(enemyVel.x);
			}
			if (enemyPos[e].y < wall[DOWN]->GetPosition().y)
			{
				enemyPos[e].y = wall[DOWN]->GetPosition().y;
			}
			enemy[e]->SetPosition(enemyPos[e]);

			enemy[e]->SetVelocity(enemyVel);
		}
	}

#pragma endregion

#pragma region プレイヤー弾と壁当たり判定

	//壁の当たり判定
	for (auto& m : playerBullets)
	{
		if (m->GetPosition().x < wall[LEFT]->GetPosition().x || wall[RIGHT]->GetPosition().x < m->GetPosition().x)
		{
			m->OnCollision();
		}
		else if (m->GetPosition().z < wall[BACK]->GetPosition().z || wall[FRONT]->GetPosition().z < m->GetPosition().z)
		{
			m->OnCollision();
		}
	}

#pragma endregion

#pragma region エネミー弾と壁当たり判定

	for (int e = 0; e < 6; e++)
	{
		const std::vector<std::unique_ptr<Bullet>>& enemyrBullets = enemy[e]->GetEnemyBullet();
		for (auto& m : enemyrBullets)
		{
			if (m->GetPosition().x < wall[LEFT]->GetPosition().x || wall[RIGHT]->GetPosition().x < m->GetPosition().x)
			{
				m->OnCollision();
			}
			else if (m->GetPosition().z < wall[BACK]->GetPosition().z || wall[BACK]->GetPosition().z < m->GetPosition().z)
			{
				m->OnCollision();
			}
		}
	}

#pragma endregion
}
