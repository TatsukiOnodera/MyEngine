#include "OnoGame.h"
#include "SceneFactory.h"

void OnoGame::Initialize()
{
	//���N���X�̏�����
	FrameWork::Initialize();

	//�V�[���̏�����
	scene_factory.reset(new SceneFactory);
	SceneManager::GetInstance()->SetSceneFactory(scene_factory.get());
	//�V�[���Z�b�g
	SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
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