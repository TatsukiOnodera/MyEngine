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

	// �T�E���h�t�@�C���ǂݍ���
	//audio->LoadSound("Resources");

	// �X�v���C�g�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/BackScreen.png");
	Sprite::LoadTexture(2, L"Resources/Reticle.png");
	Sprite::LoadTexture(3, L"Resources/PlayerHP.png");

	// ���C�g����
	lightGroup.reset(LightGroup::Create());
	Object3d::SetGroupLight(lightGroup.get());

	// �f�o�b�N�e�L�X�g
	debugText.Initialize(fontNumber);

	// �p�[�e�B�N��
	explosion.reset(ParticleManager::Create("Particle/FireParticle.png"));

	// �X�v���C�g
	reticle.reset(Sprite::Create(2, { 0, 0 }, { 0.5f, 0.5f }));
	HP.reset(Sprite::Create(3));
	HP->SetPosition({ WinApp::window_width / 2 - 256 / 2, 8});
	HP->SetColor({ 0, 0.6f, 0.5f, 0.8f });

	// OBJ�I�u�W�F�N�g
	ground.reset(Object3d::Create("Wall"));
	for (int e = 0; e < 3; e++)
	{
		enemy.emplace_back(new Enemy);
	}

	// FBX�I�u�F�N�g
	player.reset(new Player);

	// ���̑�
	lockList.reset(new AutoLockOn);

	// �I�[�f�B�I
	//audio->Initialize();

	// �ϐ��̏�����
	InitializeVariable();

	// ����������
	srand((unsigned int)time(NULL));
}

void GamePlayScene::InitializeVariable()
{
	// �n��
	float size = 1000;
	ground->SetPosition({ 0, -size, 0 });
	ground->SetRotation({ -90, 0, 0 });
	ground->SetScale({ size, size, size });
	ground->Update();

	// ���C�g
	//lightGroup->SetDirLightActive(0, true);

	/*lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightAtten(0, { 0.01f, 0.01f, 0.01f });
	lightGroup->SetPointLightColor(0, { 1, 1, 1 });
	lightGroup->SetPointLightPos(0, { 0, -950 ,-100 });*/

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

	// �I�[�g���b�N
	if (input->TriggerButton(BUTTON::U_CROSS))
	{
		lockList->ChangeTargetLock();
	}
	
	// �Փ˔���
	CheckAllCollisions();

	// HP
	HP->SetSize({ 256.0f / 20 * player->GetPlayerHP(), 72});

#pragma endregion

#pragma region �J�����ƃ��C�g�̍X�V

	// ���C�g�X�V
	lightGroup->Update();
	
	// �J�����X�V
	camera->Update();

#pragma endregion
}

void GamePlayScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	// �e�`��
	//DrawBackSprite(cmdList);
	DrawObjects(cmdList);
	DrawEffect(cmdList);
	DrawUI(cmdList);
	//DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	// �O�i�X�v���C�g�`��
	Sprite::PreDraw(cmdList);



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
	explosion->Draw(cmdList);
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	// �f�o�b�O�e�L�X�g�`��
	debugText.Draw(cmdList);
}

const float GamePlayScene::Length(const XMFLOAT3& pos1, const XMFLOAT3& pos2)
{
	XMFLOAT3 len = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}

void GamePlayScene::CheckAllCollisions()
{
#pragma region �v���C���[�ƃG�l�~�[�̏Փ˔���

	CheckPlayer2Enemy();

#pragma endregion

#pragma region �v���C���[�e�ƃG�l�~�[�̏Փ˔���

	CheckPlayerBullets2Enemy();

#pragma endregion

#pragma region �v���C���[�ƃG�l�~�[�e�̏Փ˔���

	CheckPlayer2EnemyBullets();

#pragma endregion

#pragma region �v���C���[�ƕǂ̏Փ˔���

	CheckPlayer2Wall();

#pragma endregion

#pragma region �G�l�~�[�ƕǂ̏Փ˔���

	CheckEnemy2Wall();

#pragma endregion

#pragma region �v���C���[�e�ƕǂ̏Փ˔���

	CheckPlayerBullets2Wall();

#pragma endregion

#pragma region �G�l�~�[�e�ƕǂ̏Փ˔���

	CheckEnemyBullets2Wall();

#pragma endregion
}

void GamePlayScene::CheckPlayer2Enemy()
{
	// �˒����ɂ�����̂�T��
	for (auto& m : enemy)
	{
		if (m->GetAlive() == true)
		{
			// ���b�N
			if (Length(player->GetPosition(), m->GetPosition()) < 100.0f && camera->ObjectComeInSight(m->GetPosition()) && m->GetAlive() == true)
			{
				lockList->LockOn(m.get());
			}
			else
			{
				lockList->Lost(m.get());
			}
			if (Length(player->GetPosition(), m->GetPosition()) < 150.0f && m->GetAlive() == true)
			{
				// �G�l�~�[�̔���
				//enemy[e]->ShotBullet(playerPos);
			}
		}
	}

	lockList->Update();

	// �������b�N���ĂȂ��Ȃ�
	if (lockList->GetTargetNum() == 0)
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
		reticle->SetPosition(camera->Convert3DPosTo2DPos(lockList->GetTargetEnemy()->GetPosition()));
		player->SetTargetPosition(lockList->GetTargetEnemy()->GetPosition());
		player->SetIsLock(true);
		reticle->SetInvisible(false);
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
				if (Length(pB->GetPosition(), e->GetPosition()) < 3.0f && e->GetAlive() == true)
				{
					pB->SetAlive(false);
					e->SetAlive(false);
					lockList->Lost(e.get());

					XMFLOAT3 vel = {};
					for (int i = 0; i < 360; i++)
					{
						vel.x += cosf(static_cast<float>(rand() % 360)) * 0.01f;
						vel.y += sinf(static_cast<float>(rand() % 360)) * 0.01f;
						vel.z += cosf(static_cast<float>(rand() % 360)) * 0.01f;
						explosion->Add(40, e->GetPosition(), vel, {0, 0, 0}, {0.7f, 0.4f, 0.1f, 0.1f}, {0.0f, 0.0f, 0.0f, 0.0f}, 0.2f, 2.0f, false);
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
			if (Length(eB->GetPosition(), player->GetPosition()) < 3.0f && eB->GetAlive() == true)
			{
				eB->SetAlive(false);
				if (0 < player->GetPlayerHP())
				{
					player->SetPlayerHP(player->GetPlayerHP() - 1);
				}
				XMFLOAT3 vel = {};
				for (int i = 0; i < 360; i++)
				{
					vel.x += cosf(static_cast<float>(rand() % 360)) * 0.01f;
					vel.y += sinf(static_cast<float>(rand() % 360)) * 0.01f;
					vel.z += cosf(static_cast<float>(rand() % 360)) * 0.01f;
					explosion->Add(40, player->GetPosition(), vel, {0, 0, 0}, {0.7f, 0.4f, 0.1f, 0.1f}, {0.0f, 0.0f, 0.0f, 0.0f}, 0.2f, 2.0f, false);
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

void GamePlayScene::CheckEnemy2Wall()
{
	for (auto& m : enemy)
	{
		if (m->GetAlive() == true)
		{
			XMFLOAT3 enemyPos = m->GetPosition();
			XMFLOAT3 enemyVel = m->GetVelocity();

			if (enemyPos.y < -ground->GetScale().y + 1.0f * 3)
			{
				enemyPos.y = -ground->GetScale().y + 1.0f * 3;
			}

			if (ground->GetScale().x - 5 < enemyPos.x)
			{
				enemyPos.x = ground->GetScale().x - 5;
				enemyVel.x = -fabs(enemyVel.x);
			}
			else if (enemyPos.x < -ground->GetScale().x + 5)
			{
				enemyPos.x = -ground->GetScale().x + 5;
				enemyVel.x = fabs(enemyVel.x);
			}

			if (ground->GetScale().z - 0.5f * player->GetPlayerObject()->GetScale().y < enemyPos.z)
			{
				enemyPos.z = ground->GetScale().z - 0.5f * player->GetPlayerObject()->GetScale().y;
				enemyVel.z = -fabs(enemyVel.x);
			}
			else if (enemyPos.z < -ground->GetScale().z + 0.5f * player->GetPlayerObject()->GetScale().y)
			{
				enemyPos.z = -ground->GetScale().z + 0.5f * player->GetPlayerObject()->GetScale().y;
				enemyVel.z = fabs(enemyVel.x);
			}

			m->SetPosition(enemyPos);
			m->SetVelocity(enemyVel);
		}
	}
}

void GamePlayScene::CheckPlayerBullets2Wall()
{
	//�ǂ̓����蔻��
	const std::vector<unique_ptr<Bullet>>& playerBullets = player->GetPlayerBullets();
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
	for (auto& e : enemy)
	{
		const std::vector<std::unique_ptr<Bullet>>& enemyrBullets = e->GetEnemyBullet();

		for (auto& eB : enemyrBullets)
		{
			if (eB->GetPosition().x < -ground->GetScale().x|| ground->GetScale().x < eB->GetPosition().x)
			{
				eB->SetAlive(false);
			}
			else if (eB->GetPosition().z < -ground->GetScale().z || ground->GetScale().z < eB->GetPosition().z)
			{
				eB->SetAlive(false);
			}
		}
	}
}
