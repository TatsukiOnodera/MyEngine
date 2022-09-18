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
	//audio.reset(new Audio);
	camera = Camera::GetInstance();

	// �X�v���C�g�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/background.png");
	Sprite::LoadTexture(2, L"Resources/Reticle.png");
	Sprite::LoadTexture(3, L"Resources/isAliveEnemys.png");
	Sprite::LoadTexture(4, L"Resources/Damage.png");
	Sprite::LoadTexture(5, L"Resources/Tutorial1.png");
	Sprite::LoadTexture(6, L"Resources/Tutorial2.png");

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
	sight.reset(Sprite::Create(2, { 0.0f, 0.0f }, { 0.5f, 0.5f }));
	isAliveEnemys.reset(Sprite::Create(3, { 0.0f, 0.0f }, { 0.0f, 0.0f }));
	damage.reset(Sprite::Create(4, { 0.0f, 0.0f }, { 0.5f, 0.0f }));
	tutorial1.reset(Sprite::Create(5, { 0.0f, 0.0f }, { 0.0f, 1.0f }));
	tutorial2.reset(Sprite::Create(6, { 0.0f, 0.0f }, { 1.0f, 1.0f }));

	// OBJ�I�u�W�F�N�g
	for (auto& m : defaultWall)
	{
		m.reset(Object3d::Create("Wall"));
	}
	for (int i = 0; i < 6; i++)
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
	targetNum = 0;
	listNum = 0;
	effectTimer = 0;

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
	camera->InitializeAngle();
	camera->Update();

	damage->SetPosition({ WinApp::window_width / 2, 0 });

	isAliveEnemys->SetSize({ 384, 72 });
	isAliveEnemys->SetTexSize({ 384, 72 });

	tutorial1->SetPosition({ 35, WinApp::window_height - 7 });
	tutorial1->SetSize({384, 108});
	tutorial2->SetPosition({ WinApp::window_width - 35, WinApp::window_height - 7 });
	tutorial2->SetSize({ 384, 216 });
}

void GamePlayScene::Update()
{
#pragma region �Q�[�����C���V�X�e��
	// �v���C���[
	player->Update();
	targetList.clear();
	for (int i = 0; i < enemy.size(); i++)
	{
		if (Length(enemy[i]->GetPosition(), player->GetPosition()) < 90 && enemy[i]->GetAlive())
		{
			targetList.emplace_back(i);
		}
	}
	if (input->TriggerKey(DIK_P) || input->TriggerKey(DIK_O))
	{
		listNum += input->TriggerKey(DIK_P) - input->TriggerKey(DIK_O);
	}
	if (listNum >= targetList.size())
	{
		listNum = 0;
	}
	if (targetList.size() > 0)
	{
		targetNum = targetList[listNum];
	}
	player->ShotBullet(enemy[targetNum]->GetPosition());
	for (auto& m : enemy)
	{
		if (player->bulletUpdate(m->GetPosition()))
		{
			m->SetEffectTimer();
		}
	}
	
	// �G�l�~�[
	for (auto& m : enemy)
	{
		if (m->Update(player->GetPosition()))
		{
			player->SetEffectTimer();
			effectTimer = 1;
		}
	}

	int enemyNumbers = 0;
	for (auto& m : enemy)
	{
		if (m->GetAlive())
		{
			enemyNumbers++;
		}
	}
	isAliveEnemys->SetLeftTop({ 384.0f * enemyNumbers, 0 });

	if (effectTimer > 0)
	{
		effectTimer++;
		if (effectTimer > 60)
		{
			effectTimer = 0;
		}
	}

	bool isEnd = true;
	for (auto& m : enemy)
	{
		if (m->GetAlive())
		{
			isEnd = false;
		}
	}
	if (isEnd)
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("END");
	}

	//�T�C�g
	sight->SetPosition(camera->Convert3DPosTo2DPos(enemy[targetNum]->GetPosition()));

	// �J����
	XMFLOAT2 angle = { 0, 0 };
	if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		angle.y += (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * 1;
	}
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		angle.x += (input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN)) * 1;
	}
	// �Ǐ]�J����
	camera->FollowUpCamera(player->GetPosition(), camera->GetDistance(), angle.x, angle.y);

#pragma endregion

#pragma region �J�����ƃ��C�g�̍X�V

	light->Update();
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
	DrawDebugText(cmdList);
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
	for (auto& m : defaultWall)
	{
		m->Draw();
	}

	// �G
	for (auto& m : enemy)
	{
		m->Draw();
	}

	Object3d::PostDraw();

	// FBX�I�u�W�F�N�g
	FbxObject3d::PreDraw(cmdList);

	// ���@
	player->Draw();

	FbxObject3d::PostDraw();

	// �X�v���C�g�`��
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
}

void GamePlayScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	// UI�`��
	Sprite::PreDraw(cmdList);

	//�T�C�g
	if (Length(enemy[targetNum]->GetPosition(), player->GetPosition()) < 90 && enemy[targetNum]->GetAlive())
	{
		sight->Draw();
	}

	//Enemy�̐�
	isAliveEnemys->Draw();

	//Damage�̕���
	if (effectTimer != 0)
	{
		damage->Draw();
	}

	tutorial1->Draw();
	tutorial2->Draw();

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