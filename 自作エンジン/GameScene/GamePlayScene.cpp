#include "GamePlayScene.h"
#include <time.h>
#include <stdlib.h>
#include "SafeDelete.h"

using namespace DirectX;
using namespace Microsoft::WRL;

GamePlayScene::GamePlayScene()
{

}

GamePlayScene::~GamePlayScene()
{
	safe_delete(particle);
	safe_delete(demo_back);
	safe_delete(demo_spr);
	safe_delete(chr);
	safe_delete(obj);
}

void GamePlayScene::Initialize(DirectXCommon* dx_cmd, Input* input, Audio* sound, Camera* camera)
{
	this->dx_cmd = dx_cmd;
	this->input = input;
	this->audio = sound;
	this->camera = camera;

	//�X�v���C�g�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/texture.png");
	Sprite::LoadTexture(2, L"Resources/background.png");

	//�O�i�X�v���C�g
	debugText.Initialize(fontNumber);

	//�X�v���C�g
	demo_spr = Sprite::CreateSprite(1);
	demo_back = Sprite::CreateSprite(2);

	//�I�u�W�F�N�g
	chr = Object3d::Create("chr_sword");
	obj = Object3d::Create("Bullet");

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
	obj->SetColor({1, 1, 1, 0.8f});
	obj->Update();
}

void GamePlayScene::Update()
{
	XMFLOAT3 pos = { 0, 0, 0 };
	if (input->PushKey(DIK_W))
	{
		pos.y += 0.5f;
	}
	if (input->PushKey(DIK_S))
	{
		pos.y -= 0.5f;
	}
	if (input->PushKey(DIK_D))
	{
		pos.x += 0.5f;
	}
	if (input->PushKey(DIK_A))
	{
		pos.x -= 0.5f;
	}
	camera->MoveCamera(pos);

	chr->Update();
	obj->Update();
}

void GamePlayScene::DrawBackSprite()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�O�i�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	demo_back->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);

	chr->Draw();
	obj->Draw();

	Object3d::PostDraw();

	//�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	demo_spr->Draw();

	Sprite::PostDraw();
}

void GamePlayScene::DrawUI()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//UI�`��
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void GamePlayScene::DrawParticles()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�p�[�e�B�N���`��
	particle->Draw(cmdList);
}

void GamePlayScene::DrawDebugText()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	debugText.Draw();

	Sprite::PostDraw();
}