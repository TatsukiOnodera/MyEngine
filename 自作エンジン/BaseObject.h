#pragma once
#include <fbxsdk.h>
#include <Windows.h>
#include <DirectXMath.h>
#include <SafeDelete.h>
#include <cassert>
#include <memory>
#include <array>
#include <vector>
#include "DirectXCommon.h"
#include "Input.h"
#include "InputList.h"
#include "Audio.h"
#include "Camera.h"
#include "DebugText.h"

using namespace DirectX;

class BaseObject : public InputList
{
public: // エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: //メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseObject() = default;

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
};
