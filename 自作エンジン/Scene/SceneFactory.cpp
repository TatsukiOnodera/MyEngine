#include "SceneFactory.h"
#include "TitleScene.h"
#include "GamePlayScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& scene_name)
{
	//Ÿ‚ÌƒV[ƒ“‚Ì¶¬
	BaseScene* new_scene = nullptr;

	if (scene_name == "TITLE")
	{
		new_scene = new TitleScene;
	}
	else if (scene_name == "GAMEPLAY")
	{
		new_scene = new GamePlayScene;
	}
	else
	{
		assert(0);
	}

	return new_scene;
}
