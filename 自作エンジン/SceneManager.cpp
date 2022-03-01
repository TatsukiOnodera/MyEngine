#include "SceneManager.h"
#include <SafeDelete.h>

SceneManager* SceneManager::GetInstance()
{
	static SceneManager scene_manager;

	return &scene_manager;
}

SceneManager::~SceneManager()
{
	safe_delete(scene);
}

void SceneManager::Update()
{
	//シーン切り替えがある
	if (next_scene)
	{
		//旧シーンの狩猟
		if (scene)
		{
			safe_delete(scene);
		}

		scene = next_scene;
		next_scene = nullptr;

		scene->Initialize();
	}

	scene->Update();
}

void SceneManager::Draw()
{
	scene->Draw();
}
