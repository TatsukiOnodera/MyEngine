#pragma once
#include "BaseScene.h"

/// <summary>
/// シーン管理
/// </summary>
class SceneManager
{
private: //ポインタ置き場
	//今のシーン
	BaseScene* scene = nullptr;
	//次のシーン
	BaseScene* next_scene = nullptr;

public:
	~SceneManager();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 次のシーンセット
	/// </summary>
	void SetNextScene(BaseScene* next_scene) { this->next_scene = next_scene; };
};