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
	//シーン切り替えがある
	if (next_scene)
	{
		//旧シーンの狩猟
		if (scene)
		{
			safe_delete(scene);
		}

		//シーン切り替え
		scene = next_scene;
		next_scene = nullptr;

		//新しいシーンの初期化
		scene->Initialize();
	}

	scene->Update();
}

void SceneManager::Draw()
{
	scene->Draw();
}
