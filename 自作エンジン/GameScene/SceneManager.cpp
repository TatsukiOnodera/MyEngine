#include "SceneManager.h"
#include "safeDelete.h"

void SceneManager::Update()
{
	//シーン切り替えがあるなら
	if (next_scene)
	{
		//前のシーンを終了
		if (scene)
		{
			safe_delete(scene);
		}

		//シーン切り替え
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