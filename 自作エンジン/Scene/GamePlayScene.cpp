#include "GamePlayScene.h"
#include "SceneManager.h"

#include <SafeDelete.h>
#include <cassert>
#include <fbxsdk.h>

using namespace DirectX;

GamePlayScene::~GamePlayScene()
{
	bullet.clear();
	bullet.shrink_to_fit();
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
	bullet.emplace_back(new BulletInfo(Object3d::Create("Bullet", true)));

	//FBXオブェクト
	player.reset(FbxObject3d::CreateFBXObject("player"));

	//オーディオ
	//audio->Initialize();

	//変数の初期化
	InitializeVariable();
}

void GamePlayScene::InitializeVariable()
{
	eVec = { 0.25f, 0, 0.5f };

	isDash = false;
	add0 = 0;

	intervalTime = 0;

	float fbxObjectSize = 0.5f;
	player->SetPosition({ 0, 0, -100 });
	player->SetRotation({ -90, 0, 0 });
	player->SetScale({ fbxObjectSize, fbxObjectSize, fbxObjectSize });
	player->Update();

	enemy->SetPosition({ 0, 0, 0 });
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
	if (Length(ePos, pPos) > 30)
	{
		if (ePos.z - pPos.z > 0)
		{
			eVec.z = -abs(eVec.z);
		}
		else if (ePos.z - pPos.z < 0)
		{
			eVec.z = abs(eVec.z);
		}
		if (ePos.x - pPos.x > 0)
		{
			eVec.x = -abs(eVec.x);
		}
		else if (ePos.x - pPos.x < 0)
		{
			eVec.x = abs(eVec.x);
		}
	}
	if (ePos.z > 300 || ePos.z < -300)
	{
		eVec.z = -eVec.z;
	}
	if (ePos.x > 300 || ePos.x < -300)
	{
		eVec.x = -eVec.x;
	}
	enemy->SetPosition(ePos);

	//弾
	intervalTime++;
	if (intervalTime >= 120)
	{
		intervalTime = 0;
		bool noHit = true;
		for (int i = 0; i < bullet.size(); i++)
		{
			if (bullet[i]->m_alive == false)
			{
				bullet[i]->m_alive = true;
				float s = atan2f(ePos.z - pPos.z, ePos.x - pPos.x);
				bullet[i]->m_bVec = { -cosf(s), 0, -sinf(s) };
				bullet[i]->m_bullet->SetPosition(ePos);
				bullet[i]->m_bullet->Update();
				noHit = false;
				break;
			}
		}
		if (noHit)
		{
			bullet.emplace_back(new BulletInfo(Object3d::Create("Bullet", true)));
			for (int i = 0; i < bullet.size(); i++)
			{
				if (bullet[i]->m_alive == false)
				{
					bullet[i]->m_alive = true;
					float s = atan2f(ePos.z - pPos.z, ePos.x - pPos.x);
					bullet[i]->m_bVec = { -cosf(s), 0, -sinf(s) };
					bullet[i]->m_bullet->SetPosition(ePos);
					bullet[i]->m_bullet->Update();
					break;
				}
			}
		}
	}
	for (auto& m : bullet)
	{
		if (m->m_alive)
		{
			XMFLOAT3 bPos = m->m_bullet->GetPosition();
			bPos.x += m->m_bVec.x;
			bPos.y += m->m_bVec.y;
			bPos.z += m->m_bVec.z;
			m->m_bullet->SetPosition(bPos);
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
	for (auto& m : bullet)
	{
		if (m->m_alive)
		{
			m->m_bullet->Draw(cmdList);
		}
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
