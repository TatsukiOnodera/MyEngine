#include "SceneManager.h"
#include <SafeDelete.h>

SceneManager::~SceneManager()
{
	safe_delete(scene);
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager scene_manager;

	return &scene_manager;
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

		//�V�[���؂�ւ�
		scene = next_scene;
		next_scene = nullptr;

		//�V�����V�[���̏�����
		scene->Initialize();
	}

	scene->Update();
}

void SceneManager::Draw()
{
	scene->Draw();
}
