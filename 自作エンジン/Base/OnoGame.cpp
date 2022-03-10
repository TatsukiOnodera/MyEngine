#include "OnoGame.h"
#include "SceneFactory.h"

void OnoGame::Initialize()
{
	//���N���X�̏�����
	FrameWork::Initialize();

	//�V�[���̏�����
	scene_factory = new SceneFactory;
	SceneManager::GetInstance()->SetSceneFactory(scene_factory);
	//�V�[���Z�b�g
	SceneManager::GetInstance()->ChangeScene("TITLE");
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