#include "SceneFactory.h"
#include "TitleScene.h"
#include "GamePlayScene.h"
#include "EndScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& scene_name)
{
	//次のシーンの生成
	BaseScene* new_scene = nullptr;

	if (scene_name == "TITLE")
	{
		new_scene = new TitleScene;
	}
	else if (scene_name == "GAME")
	{
		new_scene = new GamePlayScene;
	}
	else if (scene_name == "END")
	{
		new_scene = new EndScene;
	}
	else
	{
		assert(0);
	}

	return new_scene;
}
