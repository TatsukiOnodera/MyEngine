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

	// �T�E���h�t�@�C���ǂݍ���
	//audio->LoadSound("Resources");

	// �X�v���C�g�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/BackScreen.png");
	Sprite::LoadTexture(2, L"Resources/Reticle.png");
	Sprite::LoadTexture(3, L"Resources/PlayerHP.png");

	// ���C�g����
	light.reset(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightDir({ -5, -5, 0, 0 });
	Object3d::SetLight(light.get());

	// �f�o�b�N�e�L�X�g
	debugText.Initialize(fontNumber);

	// �O�i�X�v���C�g
	backScreen.reset(Sprite::Create(1));

	// �X�v���C�g
	reticle.reset(Sprite::Create(2, { 0, 0 }, { 0.5f, 0.5f }));
	HP.reset(Sprite::Create(3));
	HP->SetPosition({ WinApp::window_width / 2 - 256 / 2, 8});
	HP->SetColor({ 0, 0.6f, 0.5f, 0.8f });

	// OBJ�I�u�W�F�N�g
	ground.reset(Object3d::Create("Wall"));
	/*for (int e = 0; e < 6; e++)
	{
		enemy.emplace_back(new Enemy);
	}*/

	// FBX�I�u�F�N�g
	player.reset(new Player);

	// �I�[�f�B�I
	//audio->Initialize();

	// �ϐ��̏�����
	InitializeVariable();

	// ����������
	srand(NULL);
}

void GamePlayScene::InitializeVariable()
{
	// �n��
	float size = 1000;
	ground->SetPosition({ 0, -size, 0 });
	ground->SetRotation({ -90, 0, 0 });
	ground->SetScale({ size, size, size });
	ground->Update();

	// �J����
	camera->SetTarget({ 0, 0, 0 });
	camera->SetDistance();
	camera->Update();
}

void GamePlayScene::Update()
{
#pragma region �Q�[�����C���V�X�e��

	// �v���C���[
	player->Update();
	
	// �G�l�~�[
	for (const auto& m : enemy)
	{
		m->Update();
	}
	
	// �Փ˔���
	CheckAllCollisions();

	// HP
	HP->SetSize({ 256.0f / 20 * player->GetPlayerHP(), 72});

#pragma endregion

#pragma region �J�����ƃ��C�g�̍X�V

	// ���C�g�X�V
	light->Update();
	
	// �J�����X�V
	camera->Update();

#pragma endregion
}

void GamePlayScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	// �e�`��
	DrawBackSprite(cmdList);
	DrawObjects(cmdList);
	//DrawEffect(cmdList);
	DrawUI(cmdList);
	//DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	// �O�i�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	backScreen->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::DrawObjects(ID3D12GraphicsCommandList* cmdList)
{
	// OBJ�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);

	// ��
	ground->Draw();

	Object3d::PostDraw();

	// �v���C���[
	player->Draw(cmdList);

	// �G�l�~�[
	for (const auto& m : enemy)
	{
		m->Draw(cmdList);
	}
}

void GamePlayScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	// UI�`��
	Sprite::PreDraw(cmdList);

	reticle->Draw();
	HP->Draw();

	Sprite::PostDraw();
}

void GamePlayScene::DrawEffect(ID3D12GraphicsCommandList* cmdList)
{
	// �p�[�e�B�N���`��
	ParticleManager::PreDraw(cmdList);

	

	ParticleManager::PostDraw();
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	// �f�o�b�O�e�L�X�g�`��
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

#pragma region �v���C���[�ƃG�l�~�[�̏Փ˔���

	CheckPlayer2Enemy(playerPos, enemyPos);

#pragma endregion

#pragma region �v���C���[�e�ƃG�l�~�[�̏Փ˔���

	CheckPlayerBullets2Enemy(playerBullets, enemyPos);

#pragma endregion

#pragma region �v���C���[�ƃG�l�~�[�e�̏Փ˔���

	CheckPlayer2EnemyBullets(playerPos);

#pragma endregion

#pragma region �v���C���[�ƕǂ̏Փ˔���

	CheckPlayer2Wall(playerPos);

#pragma endregion

#pragma region �G�l�~�[�ƕǂ̏Փ˔���

	CheckEnemy2Wall(enemyPos);

#pragma endregion

#pragma region �v���C���[�e�ƕǂ̏Փ˔���

	CheckPlayerBullets2Wall(playerBullets);

#pragma endregion

#pragma region �G�l�~�[�e�ƕǂ̏Փ˔���

	CheckEnemyBullets2Wall();

#pragma endregion
}

void GamePlayScene::CheckPlayer2Enemy(const XMFLOAT3& playerPos, const XMFLOAT3* enemyPos)
{
	// �˒����ɂ�����̂�T��
	bool tmp_hit = false;
	for (int e = 0; e < enemy.size(); e++)
	{
		if (enemy[e]->GetAlive() == true)
		{
			if (Length(playerPos, enemyPos[e]) < 100.0f && camera->ObjectComeInSight(enemyPos[e]))
			{
				// �v���C���[�̔���
				targetNum = e + 1;
				tmp_hit = true;
			}
			if (Length(playerPos, enemyPos[e]) < 150.0f)
			{
				// �G�l�~�[�̔���
				enemy[e]->ShotBullet(playerPos);
			}
		}
	}
	// �������b�N���Ă��Ȃ��Ȃ�
	if (tmp_hit == false)
	{
		targetNum = 0;
	}

	// �������b�N���ĂȂ��Ȃ�
	if (targetNum == 0)
	{
		// �܂��������
		reticle->SetPosition({ 0, 0 });
		player->SetTargetPosition({ 0, 0, 0 });
		player->SetIsLock(false);
		reticle->SetInvisible(true);
	}
	else
	{
		// ���b�N��ɑł�
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
	//�ǂ̓����蔻��
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
