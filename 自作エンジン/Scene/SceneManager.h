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

public: //静的メンバ関数
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static SceneManager* GetInstance();

private: //インストラクタ
	SceneManager() = default;
	~SceneManager();
	SceneManager(const SceneManager&) = delete;
	void operator = (const SceneManager&) = delete;

public: //メンバ関数
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