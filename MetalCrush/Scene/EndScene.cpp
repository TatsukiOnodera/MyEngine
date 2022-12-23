#include "EndScene.h"
#include "SceneManager.h"
#include <time.h>
#include <stdlib.h>
#include <SafeDelete.h>

using namespace DirectX;
using namespace Microsoft::WRL;

EndScene::~EndScene()
{

}

void EndScene::Initialize()
{
	dx_cmd = DirectXCommon::GetInstance();
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	camera = Camera::GetInstance();

	//�O�i�X�v���C�g
	debugText.Initialize(0);

	//�p�[�e�B�N��
	//particle.reset(ParticleManager::Create());

	//�X�v���C�g
	endScreen.reset(Sprite::Create(13));

	//�I�u�W�F�N�g

	//�p�����[�^�[
	ResetVariable();

	//�I�[�f�B�I
	//audio->Initialize();
}

void EndScene::ResetVariable()
{

}

void EndScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void EndScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�e�`��
	//DrawBackSprite(cmdList);
	DrawObject(cmdList);
	//DrawParticles(cmdList);
	//DrawUI(cmdList);
	//DrawDebugText(cmdList);
}

void EndScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	//�O�i�X�v���C�g�`��
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void EndScene::DrawObject(ID3D12GraphicsCommandList* cmdList)
{
	//�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);



	Object3d::PostDraw();

	//�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	endScreen->Draw();

	Sprite::PostDraw();

	//FBX�I�u�W�F�N�g
	FbxObject3d::PreDraw(cmdList);



	FbxObject3d::PostDraw();
}

void EndScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	//UI�`��
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
}

void EndScene::DrawParticles(ID3D12GraphicsCommandList* cmdList)
{
	//�p�[�e�B�N���`��
	//particle->Draw();
}

void EndScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	//�f�o�b�O�e�L�X�g�`��
	debugText.Draw(cmdList);
}