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
	Sprite::LoadTexture(1, L"Resources/BackScreen.png");
	Sprite::LoadTexture(2, L"Resources/Reticle.png");
	Sprite::LoadTexture(3, L"Resources/PlayerHP.png");

	// ライト生成
	light.reset(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightDir({ -5, -5, 0, 0 });
	Object3d::SetLight(light.get());

	// デバックテキスト
	debugText.Initialize(fontNumber);

	// 前景スプライト
	backScreen.reset(Sprite::Create(1));

	// スプライト
	reticle.reset(Sprite::Create(2, { 0, 0 }, { 0.5f, 0.5f }));
	HP.reset(Sprite::Create(3));
	HP->SetPosition({ WinApp::window_width / 2 - 256 / 2, 8});
	HP->SetColor({ 0, 0.6f, 0.5f, 0.8f });

	// OBJオブジェクト
	ground.reset(Object3d::Create("Wall"));
	/*for (int e = 0; e < 6; e++)
	{
		enemy.emplace_back(new Enemy);
	}*/

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
	// 地面
	float size = 1000;
	ground->SetPosition({ 0, -size, 0 });
	ground->SetRotation({ -90, 0, 0 });
	ground->SetScale({ size, size, size });
	ground->Update();

	// カメラ
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

	// HP
	HP->SetSize({ 256.0f / 20 * player->GetPlayerHP(), 72});

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
	DrawBackSprite(cmdList);
	DrawObjects(cmdList);
	//DrawEffect(cmdList);
	DrawUI(cmdList);
	//DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	// 前景スプライト描画
	Sprite::PreDraw(cmdList);

	backScreen->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::DrawObjects(ID3D12GraphicsCommandList* cmdList)
{
	// OBJオブジェクト描画
	Object3d::PreDraw(cmdList);

	// 壁
	ground->Draw();

	Object3d::PostDraw();

	// プレイヤー
	player->Draw(cmdList);

	// エネミー
	for (const auto& m : enemy)
	{
		m->Draw(cmdList);
	}
}

void GamePlayScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	// UI描画
	Sprite::PreDraw(cmdList);

	reticle->Draw();
	HP->Draw();

	Sprite::PostDraw();
}

void GamePlayScene::DrawEffect(ID3D12GraphicsCommandList* cmdList)
{
	// パーティクル描画
	ParticleManager::PreDraw(cmdList);

	

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

#pragma region プレイヤーとエネミーの衝突判定

	CheckPlayer2Enemy(playerPos, enemyPos);

#pragma endregion

#pragma region プレイヤー弾とエネミーの衝突判定

	CheckPlayerBullets2Enemy(playerBullets, enemyPos);

#pragma endregion

#pragma region プレイヤーとエネミー弾の衝突判定

	CheckPlayer2EnemyBullets(playerPos);

#pragma endregion

#pragma region プレイヤーと壁の衝突判定

	CheckPlayer2Wall(playerPos);

#pragma endregion

#pragma region エネミーと壁の衝突判定

	CheckEnemy2Wall(enemyPos);

#pragma endregion

#pragma region プレイヤー弾と壁の衝突判定

	CheckPlayerBullets2Wall(playerBullets);

#pragma endregion

#pragma region エネミー弾と壁の衝突判定

	CheckEnemyBullets2Wall();

#pragma endregion
}

void GamePlayScene::CheckPlayer2Enemy(const XMFLOAT3& playerPos, const XMFLOAT3* enemyPos)
{
	// 射程内にいるものを探す
	bool tmp_hit = false;
	for (int e = 0; e < enemy.size(); e++)
	{
		if (enemy[e]->GetAlive() == true)
		{
			if (Length(playerPos, enemyPos[e]) < 100.0f && camera->ObjectComeInSight(enemyPos[e]))
			{
				// プレイヤーの発射
				targetNum = e + 1;
				tmp_hit = true;
			}
			if (Length(playerPos, enemyPos[e]) < 150.0f)
			{
				// エネミーの発射
				enemy[e]->ShotBullet(playerPos);
			}
		}
	}
	// 何もロックしていないなら
	if (tmp_hit == false)
	{
		targetNum = 0;
	}

	// 何もロックしてないなら
	if (targetNum == 0)
	{
		// まっすぐ飛ぶ
		reticle->SetPosition({ 0, 0 });
		player->SetTargetPosition({ 0, 0, 0 });
		player->SetIsLock(false);
		reticle->SetInvisible(true);
	}
	else
	{
		// ロック先に打つ
		reticle->SetPosition(camera->Convert3DPosTo2DPos(enemyPos[targetNum - 1]));
		player->SetTargetPosition(enemyPos[targetNum - 1]);
		player->SetIsLock(true);
		reticle->SetInvisible(false);
	}
}

void GamePlayScene::CheckPlayerBullets2Enemy(const std::vector<std::unique_ptr<Bullet>>& playerBullets, const XMFLOAT3* enemyPos)
{
	for (const auto& m : playerBullets)
	{
		if (m->GetAlive() == true)
		{
			for (int e = 0; e < enemy.size(); e++)
			{
				if (Length(m->GetPosition(), enemyPos[e]) < 5.0f && enemy[e]->GetAlive() == true)
				{
					m->SetAlive(false);
					enemy[e]->SetAlive(false);

					XMFLOAT3 pos = enemyPos[e];
					XMFLOAT3 vel = {};
					for (int i = 0; i < 10; i++)
					{
						vel.x += static_cast<float>(rand() % 21 - 10) / 10;
						vel.y += static_cast<float>(rand() % 21 - 10) / 10;
						vel.z += static_cast<float>(rand() % 21 - 10) / 10;
					}
				}
			}
		}
	}
}

void GamePlayScene::CheckPlayer2EnemyBullets(const XMFLOAT3& playerPos)
{
	for (int e = 0; e < enemy.size(); e++)
	{
		const std::vector<std::unique_ptr<Bullet>>& enemyrBullets = enemy[e]->GetEnemyBullet();

		for (const auto& m : enemyrBullets)
		{
			if (Length(m->GetPosition(), playerPos) < 5.0f && m->GetAlive() == true)
			{
				m->SetAlive(false);
				if (0 < player->GetPlayerHP())
				{
					player->SetPlayerHP(player->GetPlayerHP() - 1);
				}

				XMFLOAT3 pos = playerPos;
				XMFLOAT3 vel = {};
				for (int i = 0; i < 10; i++)
				{
					vel.x += static_cast<float>(rand() % 21 - 10) / 100;
					vel.y += static_cast<float>(rand() % 21 - 10) / 100;
					vel.z += static_cast<float>(rand() % 21 - 10) / 100;
				}
			}
		}
	}
}

void GamePlayScene::CheckPlayer2Wall(XMFLOAT3& playerPos)
{
	if (player->GetAlive() == true)
	{
		if (playerPos.y < -ground->GetScale().y + 5.0f * player->GetPlayerObject()->GetScale().z)
		{
			playerPos.y = -ground->GetScale().y + 5.0f * player->GetPlayerObject()->GetScale().z;
			player->OnLand();
		}

		if (ground->GetScale().x - 5 < playerPos.x)
		{
			playerPos.x = ground->GetScale().x - 5;
		}
		else if (playerPos.x < -ground->GetScale().x + 5)
		{
			playerPos.x = -ground->GetScale().x + 5;
		}

		if (ground->GetScale().z - 0.5f * player->GetPlayerObject()->GetScale().y < playerPos.z)
		{
			playerPos.z = ground->GetScale().z - 0.5f * player->GetPlayerObject()->GetScale().y;
		}
		else if (playerPos.z < -ground->GetScale().z + 0.5f * player->GetPlayerObject()->GetScale().y)
		{
			playerPos.z = -ground->GetScale().z + 0.5f * player->GetPlayerObject()->GetScale().y;
		}

		player->SetPosition(playerPos);
	}
}

void GamePlayScene::CheckEnemy2Wall(XMFLOAT3* enemyPos)
{
	for (int e = 0; e < enemy.size(); e++)
	{
		if (enemy[e]->GetAlive() == true)
		{
			XMFLOAT3 enemyVel = enemy[e]->GetVelocity();

			if (enemyPos[e].y < -ground->GetScale().y + 5.0f * player->GetPlayerObject()->GetScale().z)
			{
				enemyPos[e].y = ground->GetScale().y + 5.0f * player->GetPlayerObject()->GetScale().z;
			}

			if (ground->GetScale().x - 5 < enemyPos[e].x)
			{
				enemyPos[e].x = ground->GetScale().x - 5;
				enemyVel.x = -fabs(enemyVel.x);
			}
			else if (enemyPos[e].x < -ground->GetScale().x + 5)
			{
				enemyPos[e].x = -ground->GetScale().x + 5;
				enemyVel.x = fabs(enemyVel.x);
			}

			if (ground->GetScale().z - 0.5f * player->GetPlayerObject()->GetScale().y < enemyPos[e].z)
			{
				enemyPos[e].z = ground->GetScale().z - 0.5f * player->GetPlayerObject()->GetScale().y;
				enemyVel.z = -fabs(enemyVel.x);
			}
			else if (enemyPos[e].z < -ground->GetScale().z + 0.5f * player->GetPlayerObject()->GetScale().y)
			{
				enemyPos[e].z = -ground->GetScale().z + 0.5f * player->GetPlayerObject()->GetScale().y;
				enemyVel.z = fabs(enemyVel.x);
			}

			enemy[e]->SetPosition(enemyPos[e]);
			enemy[e]->SetVelocity(enemyVel);
		}
	}
}

void GamePlayScene::CheckPlayerBullets2Wall(const std::vector<std::unique_ptr<Bullet>>& playerBullets)
{
	//壁の当たり判定
	for (auto& m : playerBullets)
	{
		if (m->GetPosition().x < -ground->GetScale().x || ground->GetScale().x < m->GetPosition().x)
		{
			m->SetAlive(false);
		}
		else if (m->GetPosition().z < -ground->GetScale().z || ground->GetScale().z < m->GetPosition().z)
		{
			m->SetAlive(false);
		}
	}
}

void GamePlayScene::CheckEnemyBullets2Wall()
{
	for (int e = 0; e < enemy.size(); e++)
	{
		const std::vector<std::unique_ptr<Bullet>>& enemyrBullets = enemy[e]->GetEnemyBullet();

		for (auto& m : enemyrBullets)
		{
			if (m->GetPosition().x < -ground->GetScale().x|| ground->GetScale().x < m->GetPosition().x)
			{
				enemy[e]->SetAlive(false);
			}
			else if (m->GetPosition().z < -ground->GetScale().z || ground->GetScale().z < m->GetPosition().z)
			{
				enemy[e]->SetAlive(false);
			}
		}
	}
}
