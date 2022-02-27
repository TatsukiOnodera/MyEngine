#include "GameTitleScene.h"
#include "SafeDelete.h"

using namespace DirectX;
using namespace Microsoft::WRL;

GameTitleScene::GameTitleScene(DirectXCommon* dx_cmd, Input* input, Audio* sound, Camera* camera, SceneManager* scene_manager) : BaseScene(scene_manager)
{
	this->dx_cmd = dx_cmd;
	this->input = input;
	this->audio = sound;
	this->camera = camera;
}

GameTitleScene::~GameTitleScene()
{
	safe_delete(particle);
	safe_delete(demo_back);
	safe_delete(obj);
}

void GameTitleScene::Initialize()
{
	//�X�v���C�g�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(fontNumber, L"Resources/DebugFont/DebugFont.png");
	Sprite::LoadTexture(1, L"Resources/background.png");

	//�O�i�X�v���C�g
	debugText.Initialize(fontNumber);

	//�X�v���C�g
	demo_back = Sprite::CreateSprite(1);

	//�I�u�W�F�N�g
	obj = Object3d::Create("Bullet");

	//�p�����[�^�[
	ResetVariable();

	//�I�[�f�B�I
	audio->Initialize();
}

void GameTitleScene::ResetVariable()
{
	obj->SetPosition({10.0f, 0.0f, 0.0f});
	obj->SetScale({ 5.0f, 5.0f, 5.0f });
	obj->SetColor({1, 1, 1, 0.8f});
	obj->Update();
}

void GameTitleScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{

	}

	obj->Update();
}

void GameTitleScene::Draw()
{
	DrawBackSprite(); //�O�i�X�v���C�g
	DrawObject(); //�I�u�W�F�N�g���X�v���C�g�`��
	//DrawParticles(); //�p�[�e�B�N���`��
	//DrawUI(); //UI�`��

	//DrawDebugText(); //�f�o�b�O�e�L�X�g�`��
}

void GameTitleScene::DrawBackSprite()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�O�i�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	demo_back->Draw();

	Sprite::PostDraw();
	dx_cmd->ClearDepth();
}

void GameTitleScene::DrawObject()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);

	obj->Draw();

	Object3d::PostDraw();

	//�X�v���C�g�`��
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
}

void GameTitleScene::DrawParticles()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�p�[�e�B�N���`��
	particle->Draw(cmdList);
}

void GameTitleScene::DrawUI()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//UI�`��
	Sprite::PreDraw(cmdList);



	Sprite::PostDraw();
}

void GameTitleScene::DrawDebugText()
{
	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�X�v���C�g�`��
	Sprite::PreDraw(cmdList);

	debugText.Draw();

	Sprite::PostDraw();
}