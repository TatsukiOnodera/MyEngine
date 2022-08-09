#include "GamePlayScene.h"
#include "SceneManager.h"

#include <SafeDelete.h>
#include <cassert>
#include <fbxsdk.h>

using namespace DirectX;

GamePlayScene::~GamePlayScene()
{
	playerBullet.clear();
	playerBullet.shrink_to_fit();
	enemyBullet.clear();
	enemyBullet.shrink_to_fit();
}

void GamePlayScene::Initialize()
{
	dx_cmd = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	//audio = Audio::GetInstance();
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


	//OBJオブジェクト
	for (auto& m : defaultWall)
	{
		m.reset(Object3d::Create("Wall"));
	}
	enemy.reset(Object3d::Create("Dragon", true));
	enemyBullet.emplace_back(new Bullet(Object3d::Create("Bullet", true)));
	playerBullet.emplace_back(new Bullet(Object3d::Create("Bullet", true)));

	//FBXオブェクト
	player.reset(FbxObject3d::CreateFBXObject("player"));

	//オーディオ
	//audio->Initialize();

	//変数の初期化
	InitializeVariable();

	//乱数初期化
	srand(NULL);
}

void GamePlayScene::InitializeVariable()
{
	eVec = { static_cast<float>(rand() % 201 - 100) / 100, 0, static_cast<float>(rand() % 201 - 100) / 100 };

	isDash = false;
	add0 = 0;

	intervalTime = 0;

	player->SetPosition({ 0, 0, -100 });
	player->SetRotation({ -90, 0, 0 });
	player->SetScale({ 0.25f, 0.25f, 0.25f });
	player->Update();

	enemy->SetPosition({ 0, 0, 100 });
	enemy->SetScale({ 3, 3, 3 });
	enemy->SetColor({ 0, 0.3, 0.9, 1 });
	enemy->Update();

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
	camera->SetEye({ 0, 1, -5 });
	camera->SetDistance();
	camera->Update();
}

void GamePlayScene::Update()
{
#pragma region ゲームシステム

	//プレイヤー
	XMFLOAT3 pVec = {};
	if (input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		//ダッシュ
		if (input->TriggerKey(DIK_SPACE))
		{
			isDash = true;
			add0 = 25;

			//アニメーション
			player->PlayAnimation(false);
		}

		//ベクトル
		pVec.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 1;
		pVec.z += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 1;
		//加速するなら
		if (isDash)
		{
			pVec.x *= add0;
			pVec.z *= add0;

			add0 = add0 - 10;

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
		player->ResetAnimation();
	}
	//カメラを軸にした変換
	XMFLOAT3 pPos = player->GetPosition();
	pPos = camera->ConvertWindowPos(pPos, pVec);
	player->SetPosition(pPos);
	
	//エネミー
	XMFLOAT3 ePos = enemy->GetPosition();
	ePos.x += eVec.x;
	ePos.z += eVec.z;
	if (30 > Length(ePos, pPos))
	{
		if (ePos.x - pPos.x > 0)
		{
			eVec.x = fabs(static_cast<float>(rand() % 201 - 100) / 100);
		} 
		else if (ePos.x - pPos.x < 0)
		{
			eVec.x = -fabs(static_cast<float>(rand() % 201 - 100) / 100);
		}
		if (ePos.z - pPos.z > 0)
		{
			eVec.z = fabs(static_cast<float>(rand() % 201 - 100) / 100);
		} 
		else if (ePos.z - pPos.z < 0)
		{
			eVec.z = -fabs(static_cast<float>(rand() % 201 - 100) / 100);
		}
	}
	if (ePos.x > 300)
	{
		eVec.x = -fabs(static_cast<float>(rand() % 201 - 100) / 100);
	}
	else if (ePos.x < -300)
	{
		eVec.x = fabs(static_cast<float>(rand() % 201 - 100) / 100);
	}
	if (ePos.z > 300)
	{
		eVec.z = -fabs(static_cast<float>(rand() % 201 - 100) / 100);
	}
	else if (ePos.z < -300)
	{
		eVec.z = fabs(static_cast<float>(rand() % 201 - 100) / 100);
	}
	enemy->SetPosition(ePos);

	//弾
	//	プレイヤーの発射
	if (input->TriggerKey(DIK_RETURN))
	{
		bool noHit = true;
		for (int i = 0; i < playerBullet.size(); i++)
		{
			if (playerBullet[i]->GetAlive() == false)
			{
				playerBullet[i]->SetAlive(true);
				float s = atan2f(camera->GetEye().z - pPos.z, camera->GetEye().x - pPos.x);
				playerBullet[i]->SetVector({ -cosf(s) * 2.0f, 0, -sinf(s) * 2.0f });
				playerBullet[i]->SetPosition(pPos);
				noHit = false;
				break;
			}
		}
		if (noHit)
		{
			playerBullet.emplace_back(new Bullet(Object3d::Create("Bullet", true)));
			for (int i = 0; i < playerBullet.size(); i++)
			{
				if (playerBullet[i]->GetAlive() == false)
				{
					playerBullet[i]->SetAlive(true);
					float s = atan2f(camera->GetEye().z - pPos.z, camera->GetEye().x - pPos.x);
					playerBullet[i]->SetVector({ -cosf(s) * 2.0f, 0, -sinf(s) * 2.0f });
					playerBullet[i]->SetPosition(pPos);
					break;
				}
			}
		}
	}
	//敵の発射
	intervalTime++;
	if (intervalTime >= 120)
	{
		intervalTime = 0;
		bool noHit = true;
		for (int i = 0; i < enemyBullet.size(); i++)
		{
			if (enemyBullet[i]->GetAlive() == false)
			{
				enemyBullet[i]->SetAlive(true);
				float s = atan2f(ePos.z - pPos.z, ePos.x - pPos.x);
				enemyBullet[i]->SetVector({ -cosf(s), 0, -sinf(s) });
				enemyBullet[i]->SetPosition(ePos);
				noHit = false;
				break;
			}
		}
		if (noHit)
		{
			enemyBullet.emplace_back(new Bullet(Object3d::Create("Bullet", true)));
			for (int i = 0; i < enemyBullet.size(); i++)
			{
				if (enemyBullet[i]->GetAlive() == false)
				{
					enemyBullet[i]->SetAlive(true);
					float s = atan2f(ePos.z - pPos.z, ePos.x - pPos.x);
					enemyBullet[i]->SetVector({ -cosf(s), 0, -sinf(s) });
					enemyBullet[i]->SetPosition(ePos);
					break;
				}
			}
		}
	}
	for (auto& m : enemyBullet)
	{
		m->Update();
		if (5 > Length(m->GetPosition(), pPos) && m->GetAlive())
		{
			m->SetAlive(false);
		}
	}
	for (auto& m : playerBullet)
	{
		m->Update();
		if (5 > Length(m->GetPosition(), ePos) && m->GetAlive())
		{
			m->SetAlive(false);
		}
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

	//追従カメラ
	camera->FollowUpCamera(pPos, camera->GetDistance(), angle.x, angle.y);

#pragma endregion

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
	DrawObjects(cmdList);
	//DrawEffect(cmdList);
	//DrawUI(cmdList);
	DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	//前景スプライト描画
	Sprite::PreDraw(cmdList);

	

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
	for (auto& m : enemyBullet)
	{
		m->Draw(cmdList);
	}
	for (auto& m : playerBullet)
	{
		m->Draw(cmdList);
	}

	//FBXオブジェクト
	FbxObject3d::PreDraw(cmdList);

	player->Draw();

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

const float GamePlayScene::Length(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	XMFLOAT3 len = {pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z};

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}
