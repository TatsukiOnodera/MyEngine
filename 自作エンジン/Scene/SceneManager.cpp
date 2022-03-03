#include "SceneManager.h"
#include <SafeDelete.h>

SceneManager::~SceneManager()
{
	safe_delete(scene);
}

void SceneManager::Update()
{
	//�V�[���؂�ւ�������
	if (next_scene)
	{
		//���V�[���̎��
		if (scene)
		{
			safe_delete(scene);
		}

		scene = next_scene;
		next_scene = nullptr;

		scene->SetSceneManager(this);
		scene->Initialize();
	}

	scene->Update();
}

void SceneManager::Draw()
{
	scene->Draw();
}
