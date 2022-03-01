#include "TitleScene.h"
#include <time.h>
#include <stdlib.h>
#include "SafeDelete.h"

using namespace DirectX;
using namespace Microsoft::WRL;

TitleScene::TitleScene()
{

}

TitleScene::~TitleScene()
{
	safe_delete(particle);
}

void TitleScene::Initialize(DirectXCommon* dx_cmd, Input* input, Audio* sound, Camera* camera)
{
	this->dx_cmd = dx_cmd;
	this->input = input;
	this->audio = sound;
	this->camera = camera;

	//�X�v���C�g�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");

	//�O�i�X�v���C�g
	debugText.Initialize(fontNumber);

	//�X�v���C�g

	//�I�u�W�F�N�g

	//�p�����[�^�[
	ResetVariable();

	//�I�[�f�B�I
	audio->Initialize();
}

void TitleScene::ResetVariable()
{
	
}

void TitleScene::Update()
{
	
}

void TitleScene::Draw()
{
	DrawBackSprite();
	DrawObject();
	DrawParticles();
	DrawUI();
	DrawDebugText();
}

void TitleScene::DrawBackSprite()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�O�i�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void TitleScene::DrawObject()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);

	

	Object3d::PostDraw();

	//�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void TitleScene::DrawUI()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//UI�`��
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void TitleScene::DrawParticles()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�p�[�e�B�N���`��
	//particle->Draw(cmdList);
}

void TitleScene::DrawDebugText()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	//debugText.Draw();

	Sprite::PostDraw();
}