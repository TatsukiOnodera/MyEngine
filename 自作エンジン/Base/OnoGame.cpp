#include "OnoGame.h"
#include "GamePlayScene.h"

void OnoGame::Initialize()
{
	//���N���X�̏�����
	FrameWork::Initialize();

	//�V�[���̏�����
	game_scene = new GamePlayScene;
	game_scene->Initialize(dx_cmd, input, audio, camera);
}

void OnoGame::Finalize()
{
	//�V�[���̊J��
	safe_delete(game_scene);
	
	//���N���X�̏�����
	FrameWork::Finalize();
}

void OnoGame::Update()
{
	//���N���X�̍X�V
	FrameWork::Update();
	
	//�V�[���̍X�V
	game_scene->Update();
}

void OnoGame::Draw()
{
	dx_cmd->PreDraw();

	game_scene->Draw();

	dx_cmd->PostDraw();
}