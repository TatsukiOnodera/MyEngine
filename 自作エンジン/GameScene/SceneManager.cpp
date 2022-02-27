#include "SceneManager.h"
#include "safeDelete.h"

void SceneManager::Update()
{
	//�V�[���؂�ւ�������Ȃ�
	if (next_scene)
	{
		//�O�̃V�[�����I��
		if (scene)
		{
			safe_delete(scene);
		}

		//�V�[���؂�ւ�
		scene = next_scene;
		next_scene = nullptr;

		scene->Initialize();
	}

	scene->Update();
}

void SceneManager::Draw()
{
	scene->Update();
}