#include "OnoGame.h"
#include "GamePlayScene.h"

void OnoGame::Initialize()
{
	//���N���X�̏�����
	FrameWork::Initialize();

	//�Q�[���V�[��������
	game_scene = new GamePlayScene;
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

	game_scene->DrawBackSprite(); //�O�i�X�v���C�g
	game_scene->Draw(); //�I�u�W�F�N�g���X�v���C�g�`��
	//game_scene->DrawParticles(); //�p�[�e�B�N���`��
	game_scene->DrawUI(); //UI�`��

	game_scene->DrawDebugText(); //�f�o�b�O�e�L�X�g�`��

	dx_cmd->PostDraw();
}