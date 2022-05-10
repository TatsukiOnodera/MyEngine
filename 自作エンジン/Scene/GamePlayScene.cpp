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
	demo_back.reset(Sprite::CreateSprite(1));

	//OBJ�I�u�W�F�N�g
	/*chr.reset(Object3d::Create("chr_sword"));
	player.reset(Object3d::Create("bullet", true));*/

	//FBX�I�u�F�N�g
	fbxObject.reset(FbxObject3d::CreateFBXObject("boneTest"));

	//�p�����[�^�[
	ResetVariable();

	//�I�[�f�B�I
	audio->Initialize();
}

void GamePlayScene::ResetVariable()
{
	/*angleX = 0;
	angleY = 0;

	chr->SetPosition({ 0, 0, 10 });
	chr->Update();

	player->SetPosition({ 0, 0, 0 });
	player->Update();*/

	fbxObject->SetPosition({ 0, 0, 0 });
	fbxObject->SetScale({ 1, 1, 1 });
	fbxObject->Update();

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 5, -10 });
	camera->Update();
}

void GamePlayScene::Update()
{
	//{
	//	//���@�ړ�
	//	XMFLOAT3 pos = player->GetPosition();
	//	XMFLOAT3 vec = { 0, 0, 0 };
	//	if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
	//	{
	//		vec.x += (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * 0.25f;
	//	}
	//	if (input->PushKey(DIK_W) || input->PushKey(DIK_S))
	//	{
	//		vec.z += (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * 0.25f;
	//	}
	//	pos = camera->ConvertWindowPos(pos, vec, angleY);
	//	player->SetPosition(pos);
	//
	//	//�J������]
	//	if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	//	{
	//		angleY += (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * 2;
	//	}
	//	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	//	{
	//		angleX += (input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN)) * 2;
	//	}
	//	XMFLOAT3 eye = camera->FollowUpCamera(pos, { 0, 1, -10 }, angleX, angleY);
	//	camera->SetEye(eye);
	//
	//	//���Z�b�g
	//	if (input->TriggerKey(DIK_SPACE))
	//	{
	//		ResetVariable();
	//	}
	//}

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

	/*chr->Draw();
	player->Draw();*/

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