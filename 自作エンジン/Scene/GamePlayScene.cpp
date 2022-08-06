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
	
	//�X�v���C�g�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/background.png");

	//���C�g����
	light.reset(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightDir({-5, -5, 0, 0});
	Object3d::SetLight(light.get());

	//�O�i�X�v���C�g
	debugText.Initialize(fontNumber);

	//�p�[�e�B�N��
	//particle.reset(ParticleManager::Create());

	//�X�v���C�g


	//OBJ�I�u�W�F�N�g
	for (auto& m : defaultWall)
	{
		m.reset(Object3d::Create("Wall"));
	}
	enemy.reset(Object3d::Create("Dragon", true));
	bullet.emplace_back(new BulletInfo(Object3d::Create("Bullet", true)));

	//FBX�I�u�F�N�g
	player.reset(FbxObject3d::CreateFBXObject("player"));

	//�I�[�f�B�I
	//audio->Initialize();

	//�ϐ��̏�����
	InitializeVariable();

	//����������
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
#pragma region �Q�[���V�X�e��

	//�v���C���[
	XMFLOAT3 pVec = {};
	if (input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		//�_�b�V��
		if (input->TriggerKey(DIK_SPACE))
		{
			isDash = true;
			add0 = 25;

			//�A�j���[�V����
			player->PlayAnimation(false);
		}

		//�x�N�g��
		pVec.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 1;
		pVec.z += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 1;
		//��������Ȃ�
		if (isDash)
		{
			pVec.x *= add0;
			pVec.z *= add0;

			add0 = add0 - 10;

			//�����x��0�ɂȂ�����
			if (add0 <= 0)
			{
				add0 = 0;
				isDash = false;
			}
		}
	}
	else
	{
		//�A�j���[�V����
		player->ResetAnimation();
	}
	//�J���������ɂ����ϊ�
	XMFLOAT3 pPos = player->GetPosition();
	pPos = camera->ConvertWindowPos(pPos, pVec);
	player->SetPosition(pPos);
	
	//�G�l�~�[
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

	//�e
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
			if (5 > Length(bPos, pPos))
			{
				m->m_alive = false;
			}
		}
	}

	//�J����
	XMFLOAT2 angle = { 0, 0 };
	if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		angle.y += (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * 1;
	}
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		angle.x += (input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN)) * 1;
	}

	//�Ǐ]�J����
	camera->FollowUpCamera(pPos, camera->GetDistance(), angle.x, angle.y);

#pragma endregion

#pragma region �J�����ƃ��C�g�̍X�V

	light->Update();
	camera->Update();

#pragma endregion
}

void GamePlayScene::Draw()
{
	//�R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�e�`��
	//DrawBackSprite(cmdList);
	DrawObjects(cmdList);
	//DrawEffect(cmdList);
	//DrawUI(cmdList);
	DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	//�O�i�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::DrawObjects(ID3D12GraphicsCommandList* cmdList)
{
	//OBJ�I�u�W�F�N�g�`��
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

	//FBX�I�u�W�F�N�g
	FbxObject3d::PreDraw(cmdList);

	player->Draw();

	FbxObject3d::PostDraw();

	//�X�v���C�g�`��
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
}

void GamePlayScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	//UI�`��
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void GamePlayScene::DrawEffect(ID3D12GraphicsCommandList* cmdList)
{
	//�p�[�e�B�N���`��
	//particle->Draw(cmdList);
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	//�f�o�b�O�e�L�X�g�`��
	debugText.Draw(cmdList);
}

const float GamePlayScene::Length(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	XMFLOAT3 len = {pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z};

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}
