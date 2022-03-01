#pragma once
#include "FrameWork.h"

class GamePlayScene;
class TitleScene;

class OnoGame : public FrameWork
{
private: //ポインタ置き場
	//GamePlayScene* game_scene = nullptr;
	TitleScene* scene = nullptr;

public: //関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
};