#pragma once
#include "InputList.h"

// 前方宣言
class SceneManager;

/// <summary>
/// シーン基底
/// </summary>
class BaseScene
{
protected: //ポインタ置き場
	//シーンマネージャー（借りてくる）
	SceneManager* scene_manager = nullptr;

public:
	virtual ~BaseScene() = default;

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;
};