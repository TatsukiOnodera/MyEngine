#include "OnoGame.h"
#include "GamePlayScene.h"
#include "GameTitleScene.h"

void OnoGame::Initialize()
{
	//���N���X�̏�����
	FrameWork::Initialize();

	//�Q�[���V�[��������
	BaseScene *scene = new GameTitleScene(dx_cmd, input, audio, camera, scene_manager);
	//�V�[���}�l�[�W���[�ɃV�[�����Z�b�g
	scene_manager->SetNextScene(scene);
}

void OnoGame::Finalize()
{
	//���N���X�̊J��
	FrameWork::Finalize();
}

void OnoGame::Update()
{
	FrameWork::Update();
}

void OnoGame::Draw()
{
	FrameWork::Draw();
}