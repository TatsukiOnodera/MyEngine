#pragma once
#include "BaseScene.h"
#include <string>

/// <summary>
/// シーン構造（概念）
/// </summary>
class AbstractSceneFactory
{
public:
	//デストラクタ
	virtual ~AbstractSceneFactory() = default;

	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="scene_name">シーン名</param>
	/// <returns>生成したシーン</returns>
	virtual BaseScene* CreateScene(const std::string& scene_name) = 0;
};