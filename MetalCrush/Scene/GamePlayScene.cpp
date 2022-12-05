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

	// �T�E���h�t�@�C���ǂݍ���
	//audio->LoadSound("Resources");

	// �X�v���C�g�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	//Sprite::LoadTexture(1, L"Resources/BackScreen.png");
	Sprite::LoadTexture(2, L"Resources/Reticle.png");
	Sprite::LoadTexture(3, L"Resources/ParameterBar.png");
	Sprite::LoadTexture(4, L"Resources/NumberText.png");

	// ���C�g����
	lightGroup.reset(LightGroup::Create());
	Object3d::SetGroupLight(lightGroup.get());

	// �f�o�b�N�e�L�X�g
	debugText.Initialize(fontNumber);

	// �p�[�e�B�N��
	explosion.reset(ParticleManager::Create("Particle/FireParticle.png"));

	// �X�v���C�g
	reticle.reset(Sprite::Create(2, { 0, 0 }, { 0.5f, 0.5f }));
	/*HP.reset(Sprite::Create(3, { 0, 0 }, { 0.5f, 0.5f }));
	HP->SetPosition({ WinApp::window_width / 2, WinApp::window_height - 72});
	HP->SetColor({ 0, 0.6f, 0.5f, 0.8f });
	for (int i = 0; i < 2; i++)
	{
		ammoNum[i].reset(Sprite::Create(4, {0, 0}, {0.5f, 0.5f}));
		ammoNum[i]->SetPosition({ WinApp::window_width - 136 + (64.0f * i), WinApp::window_height - 72 });
		ammoNum[i]->SetTexSize({ 64, 72 });
		ammoNum[i]->SetSize({ 64, 72 });
		ammoNum[i]->SetColor({ 0, 0.6f, 0.5f, 0.8f });
	}*/

	// �I�u�W�F�N�g
	desertModel.reset(Model::Create("Desert", true));
	skyWallModel.reset(Model::Create("SkyWall"));
	playerModel.reset(Model::Create("Body"));
	enemyModel.reset(Model::Create("Enemy"));
	bulletModel.reset(Model::Create("Bullet", true));

	//desert.reset(TouchableObject::Create(desertModel.get()));
	desert.reset(Object3d::Create(desertModel.get()));
	skyWall.reset(Object3d::Create(skyWallModel.get()));
	for (int i = 0; i < 6; i++)
	{
		enemy.emplace_back(new Enemy(enemyModel.get(), bulletModel.get()));
	}
	player.reset(new Player(playerModel.get(), bulletModel.get()));
	p.reset(Object3d::Create(playerModel.get()));

	// ���̑�
	lockList.reset(new AutoLockOn);
	collisionManager = CollisionManager::GetInstance();

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
	desert->SetRotation({ -90, -90, 0 });
	desert->SetScale({ 0.1f, 0.1f, 0.1f });
	desert->Update();

	p->SetPosition({ 0, 5, 0 });
	p->Update();

	// ��
	skyWall->SetRotation({ 0, -90, 0});
	skyWall->SetScale({ 1000 * desert->GetScale().x, 50, 1000 * desert->GetScale().z });
	skyWall->Update();

	// ���C�g
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, { 0, -0.5f, 0, 1 });
	lightGroup->SetDirLightColor(0, { 1.0f, 1.0f, 1.0f });
	// �e
	for (int i = 0; i < 4; i++)
	{
		lightGroup->SetCircleShadowActive(i, true);
		lightGroup->SetCircleShadowDir(i, { -1, -1, 0 });
		lightGroup->SetCircleShadowAtten(i, { 0.5f, 0.6f, 0.0f });
		lightGroup->SetCircleShadowFactorAngleCos(i, { 0.0f, 0.5f });
	}
	lightGroup->Update();

	// �J����
	camera->SetCameraAngles();
	camera->SetTarget({ 0, 0, 0 });
	camera->SetDistance();
	camera->UpdateMatView();
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

	// HP�iHUD�j
	//HP->SetSize({ 256.0f / 20 * player->GetPlayerHP(), 72});

	// ����
	/*int tmp_num = player->GetBulletCapacity();
	ammoNum[0]->SetLeftTop({ static_cast<float>(tmp_num / 10) * 64, 0 });
	tmp_num %= 10;
	ammoNum[1]->SetLeftTop({ static_cast<float>(tmp_num) * 64, 0 });*/

	// �e
	lightGroup->SetCircleShadowCasterPos(0, player->GetPosition());
	for (int i = 0; i < enemy.size(); i++)
	{
		lightGroup->SetCircleShadowCasterPos(i + 1, enemy[i]->GetPosition());
	}

#pragma endregion

#pragma region �J�����ƃ��C�g�̍X�V

	// ���C�g�X�V
	lightGroup->Update();
	
	// �J�����X�V
	camera->UpdateMatView();

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
	DrawHUD(cmdList);
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

	// �n��
	desert->Draw();
	// ��
	skyWall->Draw();

	p->Draw();

	Object3d::PostDraw();

	// �v���C���[
	player->Draw(cmdList);

	// �G�l�~�[
	for (const auto& m : enemy)
	{
		m->Draw(cmdList);
	}
}

void GamePlayScene::DrawHUD(ID3D12GraphicsCommandList* cmdList)
{
	// HUD�`��
	Sprite::PreDraw(cmdList);

	reticle->Draw();
	/*HP->Draw();
	for (auto& m : ammoNum)
	{
		m->Draw();
	}*/

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

void GamePlayScene::CheckAllCollisions()
{
#pragma region �v���C���[�ƕǂ̏Փ˔���

	CheckPlayer2Wall();
	collisionManager->CheckAllCollisions();

#pragma endregion

#pragma region �G�l�~�[�ƕǂ̏Փ˔���

	CheckEnemy2Wall();

#pragma endregion

#pragma region �v���C���[�ƃG�l�~�[�̏Փ˔���

	CheckPlayer2Enemy();

#pragma endregion

#pragma region �v���C���[�e�ƃG�l�~�[�̏Փ˔���

	CheckPlayerBullets2Enemy();

#pragma endregion

#pragma region �v���C���[�ƃG�l�~�[�e�̏Փ˔���

	CheckPlayer2EnemyBullets();

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
			if (Tool::Length3to3(player->GetPosition(), m->GetPosition()) < 100.0f && camera->ObjectComeInSight(m->GetPosition()) && m->GetAlive() == true)
			{
				lockList->LockOn(m.get());
			}
			else
			{
				lockList->Lost(m.get());
			}
			if (Tool::Length3to3(player->GetPosition(), m->GetPosition()) < 350.0f && m->GetAlive() == true)
			{
				// �G�l�~�[�̔���
				m->ShotBullet(player->GetPosition());
			}
		}
	}

	lockList->Update();

	// �������b�N���ĂȂ��Ȃ�
	if (lockList->GetTargetNum() == 0)
	{
		// �܂��������
		reticle->SetPosition({ 0, 0 });
		reticle->SetInvisible(true);

		player->SetTarget(false);
	}
	else
	{
		// ���b�N��ɑł�
		reticle->SetPosition(camera->Convert3DPosTo2DPos(lockList->GetTargetEnemy()->GetPosition()));
		reticle->SetInvisible(false);

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
				if (Tool::Length3to3(pB->GetPosition(), e->GetPosition()) < 10.0f && e->GetAlive() == true)
				{
					pB->SetAlive(false);
					e->SetAlive(false);
					lockList->Lost(e.get());

					XMFLOAT3 vel = {};
					for (int i = 0; i < 360; i++)
					{
						vel.x += cosf(static_cast<float>(rand() % 21 - 10)) * 0.01f;
						vel.y += sinf(static_cast<float>(rand() % 11)) * 0.01f;
						vel.z += cosf(static_cast<float>(rand() % 21 - 10)) * 0.01f;
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
			if (Tool::Length3to3(eB->GetPosition(), player->GetPosition()) < 3.0f && eB->GetAlive() == true)
			{
				eB->SetAlive(false);
				if (0 < player->GetPlayerHP())
				{
					player->SetPlayerHP(player->GetPlayerHP() - 1);
				}
				XMFLOAT3 vel = {};
				for (int i = 0; i < 360; i++)
				{
					vel.x += cosf(static_cast<float>(rand() % 21 - 10)) * 0.01f;
					vel.y += sinf(static_cast<float>(rand() % 11)) * 0.01f;
					vel.z += cosf(static_cast<float>(rand() % 21 - 10)) * 0.01f;
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
