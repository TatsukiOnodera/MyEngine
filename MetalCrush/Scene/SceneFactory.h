#pragma once
#include "AbstractSceneFactory.h"

//ゲーム用のシーン工場
class SceneFactory : public AbstractSceneFactory
{
public: // メンバ関数
	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="scene_name">シーン名</param>
	/// <returns>生成したシーン</returns>
	BaseScene* CreateScene(const std::string& scene_name) override;
};