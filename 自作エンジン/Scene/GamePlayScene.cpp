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


	// ���C�g����
	light.reset(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightDir({ -5, -5, 0, 0 });
	Object3d::SetLight(light.get());

	// �O�i�X�v���C�g
	debugText.Initialize(fontNumber);

	// �p�[�e�B�N��
	particle.reset(ParticleManager::Create("Default/effect1.png"));

	// �X�v���C�g


	// OBJ�I�u�W�F�N�g
	for (auto& m : wall)
	{
		m.reset(Object3d::Create("Wall"));
	}
	for (int e = 0; e < 6; e++)
	{
		enemy.emplace_back(new Enemy);
	}

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
	for (auto& m : wall)
	{
		m->Draw();
	}

	Object3d::PostDraw();

	// FBX�I�u�W�F�N�g
	FbxObject3d::PreDraw(cmdList);



	FbxObject3d::PostDraw();

	// �v���C���[
	player->Draw(cmdList);

	// �G�l�~�[
	for (const auto& m : enemy)
	{
		m->Draw(cmdList);
	}

	// �X�v���C�g�`��
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
}

void GamePlayScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	// UI�`��
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void GamePlayScene::DrawEffect(ID3D12GraphicsCommandList* cmdList)
{
	// �p�[�e�B�N���`��
	ParticleManager::PreDraw(cmdList);

	particle->Draw();

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
	// �v���C���[�ƃG�l�~�[�̍��W
	XMFLOAT3 playerPos = player->GetPosition();
	XMFLOAT3 enemyPos[6] = {};
	for (int e = 0; e < enemy.size(); e++)
	{
		enemyPos[e] = enemy[e]->GetPosition();
	}

#pragma region �v���C���[�o���b�g�ƃG�l�~�[�̓����蔻��

	

#pragma endregion

#pragma region �v���C���[�ƃG�l�~�[�o���b�g�̓����蔻��

	

#pragma endregion

#pragma region �v���C���[�ƃG�l�~�[�̓����蔻��

	for (int e = 0; e < enemy.size(); e++)
	{
		if (player->GetAlive() == true && enemy[e]->GetAlive() == true && Length(playerPos, enemyPos[e]) < 20.0f)
		{
			/*XMFLOAT3 vel = enemy[e]->GetVelocity();
			vel.x = -vel.x;
			vel.y = -vel.y;
			vel.z = -vel.z;
			enemy[e]->SetVelocity(vel);*/
		}
	}

#pragma endregion

#pragma region �v���C���[�ƕǓ����蔻��

	if (player->GetAlive() == true)
	{
		if (wall[FRONT]->GetPosition().z < playerPos.z)
		{
			playerPos.z = wall[FRONT]->GetPosition().z;
			player->OnCollision();
		}
		else if (playerPos.z < wall[BACK]->GetPosition().z)
		{
			playerPos.z = wall[BACK]->GetPosition().z;
			player->OnCollision();
		}
		if (wall[RIGHT]->GetPosition().x < playerPos.x)
		{
			playerPos.x = wall[RIGHT]->GetPosition().x;
			player->OnCollision();
		}
		else if (playerPos.x < wall[LEFT]->GetPosition().x)
		{
			playerPos.x = wall[LEFT]->GetPosition().x;
			player->OnCollision();
		}
		if (wall[UP]->GetPosition().y < playerPos.y)
		{
			playerPos.y = wall[UP]->GetPosition().y;
			player->OnCollision();
		}
		else if (playerPos.y < wall[DOWN]->GetPosition().y + 1.875f)
		{
			playerPos.y = wall[DOWN]->GetPosition().y + 1.875f;
			player->OnCollision();
		}
		player->SetPosition(playerPos);
	}

#pragma endregion

#pragma region �G�l�~�[�ƕǓ����蔻��

	for (int e = 0; e < enemy.size(); e++)
	{
		if (enemy[e]->GetAlive() == true)
		{
			if (wall[FRONT]->GetPosition().z < enemyPos[e].z)
			{
				enemyPos[e].z = wall[FRONT]->GetPosition().z;
				enemy[e]->OnCollision();
			}
			else if (enemyPos[e].z < wall[BACK]->GetPosition().z)
			{
				enemyPos[e].z = wall[BACK]->GetPosition().z;
				enemy[e]->OnCollision();
			}
			if (wall[RIGHT]->GetPosition().x < enemyPos[e].x)
			{
				enemyPos[e].x = wall[RIGHT]->GetPosition().x;
				enemy[e]->OnCollision();
			}
			else if (enemyPos[e].x < wall[LEFT]->GetPosition().x)
			{
				enemyPos[e].x = wall[LEFT]->GetPosition().x;
				enemy[e]->OnCollision();
			}
			if (wall[UP]->GetPosition().y < enemyPos[e].y)
			{
				enemyPos[e].y = wall[UP]->GetPosition().y;
				enemy[e]->OnCollision();
			}
			else if (enemyPos[e].y < wall[DOWN]->GetPosition().y)
			{
				enemyPos[e].y = wall[DOWN]->GetPosition().y;
				enemy[e]->OnCollision();
			}
			enemy[e]->SetPosition(enemyPos[e]);
		}
	}

#pragma endregion
}
