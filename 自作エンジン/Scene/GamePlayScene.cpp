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
	particle.reset(ParticleManager::Create("Default/effect1.png"));

	//�X�v���C�g


	//OBJ�I�u�W�F�N�g
	for (auto& m : defaultWall)
	{
		m.reset(Object3d::Create("Wall"));
	}
	enemy.reset(new Enemy(Object3d::Create("Dragon", true)));
	enemyBullet.emplace_back(new Bullet(Object3d::Create("Bullet", true)));
	playerBullet.emplace_back(new Bullet(Object3d::Create("Bullet", true)));

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
	isDash = false;
	add0 = 0;

	intervalTime = 0;

	player->SetPosition({ 0, 0, -100 });
	player->SetRotation({ -90, 0, 0 });
	player->SetScale({ 0.25f, 0.25f, 0.25f });
	player->Update();

	for (int i = 0; i < defaultWall.size(); i++)
	{
		float size = 100;
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
#pragma region �Q�[�����C���V�X�e��
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
	enemy->Update(pPos);

	//	�v���C���[�̔���
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

	//�G�̔���
	XMFLOAT3 ePos = enemy->GetPosition();
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
	DrawEffect(cmdList);
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
	for (auto& m : enemyBullet)
	{
		m->Draw(cmdList);
	}
	for (auto& m : playerBullet)
	{
		m->Draw(cmdList);
	}

	//FBX�I�u�W�F�N�g
	FbxObject3d::PreDraw(cmdList);

	//player->Draw();

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
	ParticleManager::PreDraw(cmdList);

	particle->Draw();

	ParticleManager::PostDraw();
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

//for (int i = 0; i < 30; i++)
	//{
	//	const float rnd_pos = 10.0f;
	//	XMFLOAT3 pos = {};
	//	pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//	pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//	pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

	//	const float rnd_vel = 1.0f;
	//	XMFLOAT3 vel = {};
	//	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//	vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	//	XMFLOAT3 acc = {};
	//	//�ǉ�
	//	particle->Add(120, pos, vel, acc, 5.0f, 0.0f);
	//}