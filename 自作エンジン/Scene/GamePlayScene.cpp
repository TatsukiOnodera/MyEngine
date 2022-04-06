#include "GamePlayScene.h"
#include "SceneManager.h"
#include <time.h>
#include <stdlib.h>
#include <SafeDelete.h>
#include <cassert>
#include <sstream>
#include <iomanip>

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
	light = Light::Create();
	light->SetLightColor({ 1, 1, 1 });
	Object3d::SetLight(light);

	//�X�v���C�g
	demo_back.reset(Sprite::CreateSprite(1));

	//�I�u�W�F�N�g
	chr.reset(Object3d::Create("chr_sword"));
	obj.reset(Object3d::Create("Bullet", true));

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
	obj->SetColor({1, 1, 1, 1});
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
	camera->MoveCamera(pos);

	XMFLOAT3 rot = obj->GetRotation();
	rot.y += 1.0f;
	chr->SetRotation(rot);
	obj->SetRotation(rot);

	//��������
	static XMVECTOR lightDir = { 0, 1, 5, 0 };

	if (input->PushKey(DIK_W))
	{
		lightDir.m128_f32[1] += 1.0f;
	} 
	else if (input->PushKey(DIK_S))
	{
		lightDir.m128_f32[1] -= 1.0f;
	}
	if (input->PushKey(DIK_D))
	{
		lightDir.m128_f32[0] += 1.0f;
	} 
	else if (input->PushKey(DIK_A))
	{
		lightDir.m128_f32[0] -= 1.0f;
	}

	light->SetLightDir(lightDir);

	std::ostringstream debugstr;
	debugstr << "lightDirFactor("
		<< std::fixed << std::setprecision(2)
		<< lightDir.m128_f32[0] << ","
		<< lightDir.m128_f32[1] << ","
		<< lightDir.m128_f32[2] << ")",
		debugText.Print(debugstr.str(), 50, 50, 1.0f);

	debugstr.str("");
	debugstr.clear();

	const XMFLOAT3& cameraPos = camera->GetEye();
	debugstr << "cameraPos("
		<< std::fixed << std::setprecision(2)
		<< cameraPos.x << ","
		<< cameraPos.y << ","
		<< cameraPos.z << ")",
		debugText.Print(debugstr.str(), 50, 70, 1.0f);
}

void GamePlayScene::Draw()
{
	//�J�����ƃ��C�g�̍X�V
	light->Update();
	camera->Update();

	ID3D12GraphicsCommandList* cmdList = dx_cmd->GetCmdList();

	//�e�`��
	DrawBackSprite(cmdList);
	DrawObject(cmdList);
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

void GamePlayScene::DrawObject(ID3D12GraphicsCommandList* cmdList)
{
	//�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);

	chr->Draw();
	obj->Draw();

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