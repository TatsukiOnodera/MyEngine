#pragma once
#include "BaseScene.h"

class SceneManager
{
private:
	//今のシーン
	BaseScene* scene = nullptr;
	BaseScene* next_scene = nullptr;

public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//次のシーンをセット
	void SetNextScene(BaseScene* next_scene) { this->next_scene = next_scene; }
};