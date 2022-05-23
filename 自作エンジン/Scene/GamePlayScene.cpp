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
	light->SetLightDir({-5, -5, 0, 0});
	Object3d::SetLight(light.get());

	//�X�v���C�g
	demo_back.reset(Sprite::CreateSprite(1));

	//OBJ�I�u�W�F�N�g
	for (int i = 0; i < wall.size(); i++)
	{
		wall[i].reset(Object3d::Create("Wall"));
	}

	//FBX�I�u�F�N�g
	fbxObject.reset(FbxObject3d::CreateFBXObject("boneTest"));
	fbxObject->PlayAnimation(true);

	//�p�����[�^�[
	ResetVariable();

	//�I�[�f�B�I
	audio->Initialize();
}

void GamePlayScene::ResetVariable()
{
	fbxObject->SetPosition({ 0, 0, 0 });
	fbxObject->Update();

	for (int i = 0; i < FIN; i++)
	{
		float size = 100;
		XMFLOAT3 pos = { 0, 0, 0 };
		XMFLOAT3 rot = { 0, 0, 0 };
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
	camera->SetEye({ 0, 5, -10 });
	camera->Update();
}

void GamePlayScene::Update()
{
	//�ړ�
	XMFLOAT3 pos = fbxObject->GetPosition();
	if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		pos.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.25f;
	}
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		pos.y += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 0.25f;
	}
	fbxObject->SetPosition(pos);

	//�J����
	XMFLOAT3 eye = {0, 0, 0};
	if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		eye.x += (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * 0.25f;
	}
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		eye.y += (input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN)) * 0.25f;
	}
	if (input->PushKey(DIK_RSHIFT) || input->PushKey(DIK_END))
	{
		eye.z += (input->PushKey(DIK_END) - input->PushKey(DIK_RSHIFT)) * 0.25f;
	}
	camera->MoveCamera(eye);
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
	DrawOthers(cmdList);
	//DrawParticle(cmdList);
	//DrawUI(cmdList);
	DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	//�O�i�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	//demo_back->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::DrawOthers(ID3D12GraphicsCommandList* cmdList)
{
	//OBJ�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);

	for (int i = 0; i < FIN; i++)
	{
		wall[i]->Draw();
	}

	Object3d::PostDraw();

	//FBX�I�u�W�F�N�g
	FbxObject3d::PreDraw(cmdList);

	fbxObject->Draw();

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
