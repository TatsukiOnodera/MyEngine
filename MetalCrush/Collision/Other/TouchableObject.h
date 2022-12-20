#pragma once
#include "Object3d.h"

/// <summary>
/// 接触可能オブジェクト
/// </summary>
class TouchableObject : public Object3d
{
public: // メンバ関数
	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <param name="model">モデルデータ</param>
	/// <param name="div">縦横を何分割にするか</param>
	/// <returns>接触可能オブジェクト</returns>
	static TouchableObject* Create(Model* model = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデルデータ</param>
	/// <returns>成否</returns>
	bool Initialize(Model* model);
};
