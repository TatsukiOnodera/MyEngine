#include "TitleScene.h"
#include "GamePlayScene.h"
#include "SceneManager.h"
#include <time.h>
#include <stdlib.h>
#include <SafeDelete.h>

using namespace DirectX;
using namespace Microsoft::WRL;

TitleScene::~TitleScene()
{
	safe_delete(particle);
}

void TitleScene::Initialize()
{
	dx_cmd = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	camera = Camera::GetInstance();

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
	if (input->TriggerKey(DIK_RETURN))
	{
		//�V�[���؂�ւ�
		BaseScene* scene = new GamePlayScene();
		scene_manager->SetNextScene(scene);
	}
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