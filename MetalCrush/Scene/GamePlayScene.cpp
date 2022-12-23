#include "GamePlayScene.h"
#include "SceneManager.h"

#include <Tool.h>
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

	// サウンドファイル読み込み
	//audio->LoadSound("Resources");

	// ライト生成
	lightGroup.reset(LightGroup::Create());
	Object3d::SetGroupLight(lightGroup.get());

	// デバックテキスト
	debugText.Initialize(0);

	// パーティクル
	explosion.reset(ParticleManager::Create("Particle/FireParticle.png"));

	// モデル
	desertModel.reset(Model::Create("Desert", true));
	skyWallModel.reset(Model::Create("SkyWall"));
	enemyModel.reset(Model::Create("Tank"));
	bulletModel.reset(Model::Create("Bullet", true));

	// OBJ
	desert.reset(TouchableObject::Create(desertModel.get()));
	//desert.reset(Object3d::Create(desertModel.get()));
	skyWall.reset(Object3d::Create(skyWallModel.get()));
	for (int i = 0; i < 10; i++)
	{
		enemy.emplace_back(new Enemy(enemyModel.get(), bulletModel.get()));
	}
	player.reset(new Player(bulletModel.get()));

	// スプライト
	Reticle.reset(Sprite::Create(1, { 0, 0 }, { 0.5f, 0.5f }));
	HPFrame.reset(Sprite::Create(3, { 0, 0 }, { 0.5f, 1.0f }));
	HP.reset(Sprite::Create(4, { 0, 0 }, { 0, 0.5f }));
	BoosterFrame.reset(Sprite::Create(5, { 0, 0 }, { 0.5f, 1.0f }));
	Booster.reset(Sprite::Create(6, { 0, 0 }, { 0, 0.5f }));
	Monitor.reset(Sprite::Create(7));
	GameOver.reset(Sprite::Create(8));
	BulletNumFrame.reset(Sprite::Create(9, { 0, 0 }, { 0.5f, 1.0f }));
	for (int i = 0; i < 2; i++)
	{
		BulletNum[i].reset(Sprite::Create(2, { 0.5f, 0.5f }, { 0.5f, 1.0f }));
	}
	Rader.reset(Sprite::Create(10, { 0, 0 }, { 0.5f, 0.5f }));
	for (int i = 0; i < enemy.size(); i++)
	{
		enemysMarker.emplace_back(Sprite::Create(11, { 0, 0 }, { 0.5f, 0.5f }));
	}

	// その他
	lockList.reset(new AutoLockOn);
	collisionManager = CollisionManager::GetInstance();

	// オーディオ
	//audio->Initialize();

	// 変数の初期化
	InitializeParameter();

	// 乱数初期化
	srand((unsigned int)time(NULL));
}

void GamePlayScene::InitializeParameter()
{
	// 地面
	desert->SetScale({ 0.1f, 0.1f, 0.1f });
	desert->Update();

	// 壁
	XMFLOAT3 scale = { desert->GetScale().x * 1000, desert->GetScale().y * 1000, desert->GetScale().z * 1000 };
	skyWall->SetScale(scale);
	skyWall->ChangeShaderPipeline(L"Texture");
	skyWall->Update();

	// ライト
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, { -1, -1, 0, 1 });
	lightGroup->SetDirLightColor(0, { 1.0f, 1.0f, 1.0f });

	// 影
	for (int i = 0; i < 4; i++)
	{
		lightGroup->SetCircleShadowActive(i, true);
		lightGroup->SetCircleShadowDir(i, { 0, -1, 0 });
		lightGroup->SetCircleShadowAtten(i, { 0.5f, 0.6f, 0.0f });
		lightGroup->SetCircleShadowFactorAngleCos(i, { 0.0f, 0.5f });
	}
	lightGroup->Update();

	// スプライト
	XMFLOAT4 spriteColor = { 1, 1, 1, 0.8f };
	HPFrame->SetPosition({ WinApp::window_width / 2, 72 * 9 });
	HPFrame->SetColor(spriteColor);
	HP->SetPosition({ WinApp::window_width / 2 - 248, 72 * 9 - 36 });
	HP->SetColor(spriteColor);
	BoosterFrame->SetPosition({ WinApp::window_width / 2, 72 * 10 });
	BoosterFrame->SetColor(spriteColor);
	Booster->SetPosition({ WinApp::window_width / 2 - 236, 72 * 10 - 36 });
	Booster->SetColor(spriteColor);
	BulletNumFrame->SetPosition({ 128.0f * 9 - 7, 72.0f * 10 - 7 });
	BulletNumFrame->SetColor(spriteColor);
	for (int i = 0; i < 2; i++)
	{
		BulletNum[i]->SetPosition({ 128.0f * 8.5f + 40.0f * i, 72.0f * 10 - 10 });
		BulletNum[i]->SetTexSize({ 64, 72 });
		BulletNum[i]->SetSize({ 64, 72 });
		BulletNum[i]->SetColor(spriteColor);
	}
	Rader->SetPosition({ 120, 600 });
	Rader->SetColor(spriteColor);

	// カメラ
	camera->SetCameraAngles();
	camera->SetTarget({ 0, 0, 0 });
	camera->SetDistance({0, 0, -5});
	camera->UpdateMatView();
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

	// オートロック
	if (input->TriggerButton(BUTTON::U_CROSS))
	{
		lockList->ChangeTargetLock();
	}
	
	// 衝突判定
	CheckAllCollisions();

	// HUD
	// ゲージ
	HP->SetSize({ 496.0f * (float)player->GetPlayerHP() / 20, 56});
	Booster->SetSize({ 472.0f * (float)player->GetBoosterGauge() / 360, 28});
	// 弾数
	int tmp = player->GetBulletNum();
	BulletNum[0]->SetLeftTop({ static_cast<float>(tmp / 10) * 64, 0 });
	tmp %= 10;
	BulletNum[1]->SetLeftTop({ static_cast<float>(tmp) * 64, 0 });
	// レーダー
	for (int i = 0; i < enemy.size(); i++)
	{
		float raderRange = 2000;
		XMFLOAT3 offset = Tool::VectorFloat3(enemy[i]->GetPosition(), player->GetPosition());
		offset.z *= -1;
		offset = camera->ConvertWindowYPos({ 0, 0, 0 }, offset);
		if (fabs(offset.x) < raderRange && fabs(offset.z) < raderRange && enemy[i]->GetAlive() == true)
		{
			enemysMarker[i]->SetInvisible(false);
			XMFLOAT2 pos = Rader->GetPostion();
			enemysMarker[i]->SetPosition({ pos.x + 120.0f * offset.x / raderRange, pos.y + 120.0f * offset.z / raderRange });
		}
		else
		{
			enemysMarker[i]->SetInvisible(true);
		}
	}

	isEnd = true;
	for (const auto& m : enemy)
	{
		if (m->GetAlive() == true)
		{
			isEnd = false;
		}
	}
	if (player->GetPlayerHP() == 0)
	{
		isEnd = true;
	}

	// 影
	lightGroup->SetCircleShadowCasterPos(0, player->GetPosition());
	/*for (int i = 0; i < enemy.size(); i++)
	{
		lightGroup->SetCircleShadowCasterPos(i + 1, enemy[i]->GetPosition());
	}*/

#pragma endregion

#pragma region カメラとライトの更新

	// ライト更新
	lightGroup->Update();
	
	// カメラ更新
	camera->UpdateMatView();

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
	DrawHUD(cmdList);
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

	// 地面
	desert->Draw();
	// 壁
	skyWall->Draw();

	Object3d::PostDraw();

	// プレイヤー
	player->Draw(cmdList);

	// エネミー
	for (const auto& m : enemy)
	{
		m->Draw(cmdList);
	}
}

void GamePlayScene::DrawHUD(ID3D12GraphicsCommandList* cmdList)
{
	// HUD描画
	Sprite::PreDraw(cmdList);

	if (isEnd == false)
	{
		HPFrame->Draw();
		HP->Draw();
		BoosterFrame->Draw();
		Booster->Draw();
		Monitor->Draw();
		BulletNumFrame->Draw();
		for (const auto& m :BulletNum)
		{
			m->Draw();
		}
		Reticle->Draw();
		Rader->Draw();
		for (const auto& m : enemysMarker)
		{
			m->Draw();
		}
	}
	else
	{
		GameOver->Draw();
	}

	Sprite::PostDraw();
}

void GamePlayScene::DrawEffect(ID3D12GraphicsCommandList* cmdList)
{
	// パーティクル描画
	explosion->Draw(cmdList);
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	// デバッグテキスト描画
	debugText.Draw(cmdList);
}

void GamePlayScene::CheckAllCollisions()
{
#pragma region コリジョンマネージャー

	collisionManager->CheckAllCollisions();

#pragma endregion

#pragma region プレイヤーと壁の衝突判定

	CheckPlayer2Wall();

#pragma endregion

#pragma region エネミーと壁の衝突判定

	CheckEnemy2Wall();

#pragma endregion

#pragma region プレイヤーとエネミーの衝突判定

	CheckPlayer2Enemy();

#pragma endregion

#pragma region プレイヤー弾とエネミーの衝突判定

	CheckPlayerBullets2Enemy();

#pragma endregion

#pragma region プレイヤーとエネミー弾の衝突判定

	CheckPlayer2EnemyBullets();

#pragma endregion
}

void GamePlayScene::CheckPlayer2Enemy()
{
	// 射程内にいるものを探す
	for (auto& m : enemy)
	{
		if (m->GetAlive() == true)
		{
			// 射程内か調べる
			if (player->SearchTarget(m->GetPosition()) && camera->ObjectComeInSight(m->GetPosition()))
			{
				lockList->LockOn(m.get());
			}
			else
			{
				lockList->Lost(m.get());
			}
			if (m->SearchTarget(player->GetPosition()))
			{
				// エネミーの発射
				m->ShotBullet(player->GetPosition());
			}
		}
	}

	// 更新
	lockList->Update();

	// 何もロックしてないなら
	if (lockList->GetTargetNum() == 0)
	{
		// まっすぐ飛ぶ
		Reticle->SetPosition({ 0, 0 });
		Reticle->SetInvisible(true);

		player->SetTarget(false);
	}
	else
	{
		// ロック先に打つ
		Reticle->SetPosition(camera->Convert3DPosTo2DPos(lockList->GetTargetEnemy()->GetPosition()));
		Reticle->SetInvisible(false);

		player->SetTarget(true, lockList->GetTargetEnemy()->GetPosition());
	}
}

void GamePlayScene::CheckPlayerBullets2Enemy()
{
	const std::vector<unique_ptr<Bullet>>& playerBullets = player->GetPlayerBullets();
	for (const auto& pB : playerBullets)
	{
		if (pB->GetAlive() == true)
		{
			for (auto& e : enemy)
			{
				if (e->GetAlive() == true && Tool::LengthFloat3(pB->GetPosition(), e->GetPosition()) < 3.0f)
				{
					pB->SetAlive(false);
					if (e->EnemyDamage(1) == true)
					{
						e->SetAlive(false);
						lockList->Lost(e.get());
					}

					XMFLOAT3 vel = {};
					for (int i = 0; i < 100; i++)
					{
						float x, y, z;
						x = static_cast<float>(rand() % 360);
						y = static_cast<float>(rand() % 360);
						z = static_cast<float>(rand() % 360);
						vel.x += cosf(XMConvertToRadians(x)) * 0.04f;
						vel.y += sinf(XMConvertToRadians(y)) * 0.04f;
						vel.z += cosf(XMConvertToRadians(z)) * 0.04f;
						explosion->Add(60, e->GetPosition(), vel, {0, 0, 0}, {0.8f, 0.5f, 0.2f, 0.1f}, {0.0f, 0.0f, 0.0f, 0.0f}, 0.2f, 10.0f, false);
					}
				}
			}
		}
	}
}

void GamePlayScene::CheckPlayer2EnemyBullets()
{
	for (auto& e : enemy)
	{
		const std::vector<std::unique_ptr<Bullet>>& enemyrBullets = e->GetEnemyBullet();

		for (const auto& eB : enemyrBullets)
		{
			if (Tool::LengthFloat3(eB->GetPosition(), player->GetPosition()) < 3.0f && eB->GetAlive() == true)
			{
				eB->SetAlive(false);
				if (0 < player->GetPlayerHP())
				{
					player->PlayerDamage(2);
				}
				XMFLOAT3 vel = {};
				for (int i = 0; i < 50; i++)
				{
					float x, y, z;
					x = static_cast<float>(rand() % 720);
					y = static_cast<float>(rand() % 720);
					z = static_cast<float>(rand() % 720);
					vel.x += cosf(XMConvertToRadians(x)) * 0.01f;
					vel.y += sinf(XMConvertToRadians(y)) * 0.01f;
					vel.z += cosf(XMConvertToRadians(z)) * 0.01f;
					explosion->Add(60, player->GetPosition(), vel, {0, 0, 0}, {0.8f, 0.5f, 0.2f, 0.1f}, {0.0f, 0.0f, 0.0f, 0.0f}, 0.2f, 2.0f, false);
				}
			}
		}
	}
}

void GamePlayScene::CheckPlayer2Wall()
{
	if (player->GetAlive() == true)
	{
		XMFLOAT3 playerPos = player->GetPosition();

		if (playerPos.y < 10 + 5.0f * player->GetPlayerObject()->GetScale().z)
		{
			playerPos.y = 10 + 5.0f * player->GetPlayerObject()->GetScale().z;
			player->HitGround();
		}

		player->SetPosition(playerPos);
	}
}

void GamePlayScene::CheckEnemy2Wall()
{
	for (auto& m : enemy)
	{
		if (m->GetAlive() == true)
		{
			XMFLOAT3 enemyPos = m->GetPosition();

			if (enemyPos.y < 10 + 0.25f * m->GetObject3d()->GetScale().y)
			{
				enemyPos.y = 10 + 0.25f * m->GetObject3d()->GetScale().y;
			}

			m->SetPosition(enemyPos);
		}
	}
}
