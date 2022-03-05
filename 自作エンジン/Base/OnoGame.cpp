#include "OnoGame.h"
#include "GamePlayScene.h"
#include "TitleScene.h"

void OnoGame::Initialize()
{
	//���N���X�̏�����
	FrameWork::Initialize();

	//�V�[���̏�����
	BaseScene* scene = new TitleScene;
	//�V�[���Z�b�g
	SceneManager::GetInstance()->SetNextScene(scene);
}

void OnoGame::Finalize()
{
	//���N���X�̏�����
	FrameWork::Finalize();
}

void OnoGame::Update()
{
	//���N���X�̍X�V
	FrameWork::Update();
}

void OnoGame::Draw()
{
	//���N���X�̍X�V
	FrameWork::Draw();
}