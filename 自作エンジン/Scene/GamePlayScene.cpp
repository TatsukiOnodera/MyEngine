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
	safe_delete(light);
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
	light = Light::Create();
	light->SetLightColor({ 1, 1, 1 });
	light->SetLightDir({0, 1, 5, 0});
	Object3d::SetLight(light);

	//�X�v���C�g
	demo_back.reset(Sprite::CreateSprite(1));

	//OBJ�I�u�W�F�N�g
	chr.reset(Object3d::Create("chr_sword"));
	obj.reset(Object3d::Create("Bullet", true));

	//FBX�I�u�F�N�g
	fbxObject.reset(FbxObject3d::CreateFBXObject("cube"));

	//�p�����[�^�[
	ResetVariable();

	//�I�[�f�B�I
	audio->Initialize();
}

void GamePlayScene::ResetVariable()
{
	chr->SetScale({5.0f, 5.0f, 5.0f});
	chr->Update();

	obj->SetPosition({10.0f, 0.0f, 0.0f});
	obj->SetScale({ 5.0f, 5.0f, 5.0f });
	obj->Update();
}

void GamePlayScene::Update()
{
	XMFLOAT3 pos = { 0, 0, 0 };
	if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		pos.x = (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * 0.5f;
	}
	else if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		pos.y = (input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN)) * 0.5f;
	}
	else if (input->PushKey(DIK_END) || input->PushKey(DIK_RSHIFT))
	{
		pos.z = (input->PushKey(DIK_END) - input->PushKey(DIK_RSHIFT));
	}
	camera->MoveCamera(pos);


}

void GamePlayScene::Draw()
{
	//�J�����ƃ��C�g�̍X�V
	light->Update();
	camera->Update();

	//�R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�e�`��
	DrawBackSprite(cmdList);
	AnyDraw(cmdList);
	//DrawParticle(cmdList);
	//DrawUI(cmdList);
	DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	//�O�i�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	demo_back->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::AnyDraw(ID3D12GraphicsCommandList* cmdList)
{
	//OBJ�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);

	

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