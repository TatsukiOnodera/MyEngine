#include "GamePlayScene.h"
#include "SceneManager.h"

#include <time.h>
#include <stdlib.h>
#include <SafeDelete.h>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <fbxsdk.h>

using namespace DirectX;
using namespace Microsoft::WRL;

GamePlayScene::~GamePlayScene()
{

}

void GamePlayScene::Initialize()
{
	dx_cmd = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	camera = Camera::GetInstance();
	
	//�X�v���C�g�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/background.png");

	//�O�i�X�v���C�g
	debugText.Initialize(fontNumber);

	//���C�g����
	light.reset(Light::Create());
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightDir({-1, -1, 0, 0});
	Object3d::SetLight(light.get());

	//�X�v���C�g
	

	//�I�u�W�F�N�g
	ballA.reset(Object3d::Create("Bullet", true));
	ballB.reset(Object3d::Create("ball", true));

	//�p�����[�^�[
	camera->SetEye({ 0, 0, -300 });
	ResetVariable();

	//�I�[�f�B�I
	audio->Initialize();
}

void GamePlayScene::ResetVariable()
{
	isStart = false;
	isCollision = false;

	v0A = 10;
	v0B = -10;

	vA = v0A;
	vB = v0B;

	mA = 7.5;
	mB = 5;

	rA = 10;
	rB = 10;

	bounce = 0.7;

	ballA->SetPosition({-200, -100, 0});
	ballB->SetPosition({ 200, -100, 0 });

	ballA->SetScale({ rA, rA, rA });
	ballB->SetScale({ rB, rB, rB });

	ballA->Update();
	ballB->Update();
}

void GamePlayScene::Update()
{
	if (input->TriggerKey(DIK_R) && isStart == false && isCollision == false)
	{
		ResetVariable();
		isStart = true;
	}

	//���W�擾
	XMFLOAT3 posA = ballA->GetPosition();
	XMFLOAT3 posB = ballB->GetPosition();

	if (isStart == true)
	{
		posA.x += vA;
		posB.x += vB;
	}
	else if (isCollision == true)
	{
		posA.x += vA;
		posB.x += vB;

		if (vA > 0)
		{
			vA -= 1;
			if (vA <= 0)
			{
				vA = 0;
			}
		}
		else
		{
			vA += 1;
			if (vA >= 0)
			{
				vA = 0;
			}
		}
		if (vB > 0)
		{
			vB -= 1;
			if (vB <= 0)
			{
				vB = 0;
			}
		}
		else
		{
			vB += 1;
			if (vB >= 0)
			{
				vB = 0;
			}
		}

		if (vA == 0 && vB == 0)
		{
			isCollision = false;
		}
	}
	if (powf(posA.x - posB.x, 2) <= powf(rA + rB, 2))
	{
		float b = bounce * vA - vB;
		float vf1 = (mA * vA + mB * vB - b * mB) / (mA + mB);
		float vf2 = b + vf1;
		vA = vf1;
		vB = vf2;
		isStart = false;
		isCollision = true;
	}

	ballA->SetPosition(posA);
	ballB->SetPosition(posB);

	debugText.Print("R : Start / ReStart", 10, 10, 2);
}

void GamePlayScene::Draw()
{
	//�J�����ƃ��C�g�̍X�V
	light->Update();
	camera->Update();

	//�R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�e�`��
	//DrawBackSprite(cmdList);
	Draw(cmdList);
	//DrawParticle(cmdList);
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

void GamePlayScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//OBJ�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);

	ballA->Draw();
	ballB->Draw();

	Object3d::PostDraw();

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

void GamePlayScene::DrawParticle(ID3D12GraphicsCommandList* cmdList)
{
	//�p�[�e�B�N���`��
	particle->Draw(cmdList);
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	//�f�o�b�O�e�L�X�g�`��
	debugText.Draw(cmdList);
}