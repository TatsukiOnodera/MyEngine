#pragma once
#include "FrameWork.h"

class OnoGame : public FrameWork
{
public: //関数
	/// <summary>
	/// リソースの読み込み
	/// </summary>
	void LoadResources();

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