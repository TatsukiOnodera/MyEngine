#include "GamePlayScene.h"
#include "SceneManager.h"
#include <time.h>
#include <stdlib.h>
#include <SafeDelete.h>

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
	Sprite::LoadTexture(1, L"Resources/texture.png");
	Sprite::LoadTexture(2, L"Resources/background.png");

	//�O�i�X�v���C�g
	debugText.Initialize(fontNumber);

	//�X�v���C�g
	demo_spr.reset(Sprite::CreateSprite(1));
	demo_back.reset(Sprite::CreateSprite(2));

	//�I�u�W�F�N�g
	chr.reset(Object3d::Create("chr_sword"));
	obj.reset(Object3d::Create("Bullet"));

	//�p�����[�^�[
	ResetVariable();

	//�I�[�f�B�I
	audio->Initialize();
}

void GamePlayScene::ResetVariable()
{
	chr->SetScale({5.0f, 5.0f, 5.0f});

	obj->SetPosition({10.0f, 0.0f, 0.0f});
	obj->SetScale({ 5.0f, 5.0f, 5.0f });
	obj->SetColor({1, 1, 1, 0.8f});
}

void GamePlayScene::Update()
{
	XMFLOAT3 pos = { input->LeftStickAngle().x / 2, input->LeftStickAngle().y / 2, 0 };
	camera->MoveCamera(pos);
}

void GamePlayScene::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�e�`��
	DrawBackSprite(cmdList);
	DrawObject(cmdList);
	//DrawParticles(cmdList);
	DrawUI(cmdList);
	//DrawDebugText(cmdList);
}

void GamePlayScene::DrawBackSprite(ID3D12GraphicsCommandList* cmdList)
{
	//�O�i�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	demo_back->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GamePlayScene::DrawObject(ID3D12GraphicsCommandList* cmdList)
{
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

void GamePlayScene::DrawUI(ID3D12GraphicsCommandList* cmdList)
{
	//UI�`��
	Sprite::PreDraw(cmdList);

	

	Sprite::PostDraw();
}

void GamePlayScene::DrawParticles(ID3D12GraphicsCommandList* cmdList)
{
	//�p�[�e�B�N���`��
	particle->Draw(cmdList);
}

void GamePlayScene::DrawDebugText(ID3D12GraphicsCommandList* cmdList)
{
	//�f�o�b�O�e�L�X�g�`��
	debugText.Draw(cmdList);
}