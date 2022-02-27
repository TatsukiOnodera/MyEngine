#pragma once

//前方宣言
class SceneManager;

class BaseScene
{
public:
	BaseScene(SceneManager* scene_manager);

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

private:
	//シーンマネージャー（借りてくる）
	SceneManager* scene_manager = nullptr;
};