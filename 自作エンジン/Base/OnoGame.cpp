#include "OnoGame.h"

void OnoGame::Initialize()
{
	//���N���X�̏�����
	FrameWork::Initialize();

	//�Q�[���V�[��������
	game_scene = new GameScene;
	game_scene->Initialize(dx_cmd, input, audio, camera);
}

void OnoGame::Finalize()
{
	//�J��
	safe_delete(game_scene);
	
	//���N���X�̊J��
	FrameWork::Finalize();
}

void OnoGame::Update()
{
	FrameWork::Update();
	
	//�Q�[���V�[��
	game_scene->Update();
}

void OnoGame::Draw()
{
	dx_cmd->PreDraw();

	game_scene->Draw();

	dx_cmd->PostDraw();
}