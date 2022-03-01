#include "OnoGame.h"
#include "GamePlayScene.h"
#include "TitleScene.h"

void OnoGame::Initialize()
{
	//���N���X�̏�����
	FrameWork::Initialize();

	//�V�[���̏�����
	scene = new TitleScene;
	scene->Initialize(dx_cmd, input, audio, camera);
}

void OnoGame::Finalize()
{
	//�V�[���̊J��
	safe_delete(scene);
	
	//���N���X�̏�����
	FrameWork::Finalize();
}

void OnoGame::Update()
{
	//���N���X�̍X�V
	FrameWork::Update();
	
	//�V�[���̍X�V
	scene->Update();
}

void OnoGame::Draw()
{
	dx_cmd->PreDraw();

	scene->Draw();

	dx_cmd->PostDraw();
}