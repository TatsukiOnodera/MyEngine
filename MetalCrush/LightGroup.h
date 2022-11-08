#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"

#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

class LightGroup
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 定数
	static const int c_pointLightNum = 3;
	static const int c_dirLightNum = 3;

public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		// 環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		// 平行光源用
		//DirectionalLight::ConstBufferData dirLights[c_dirLightNum];
		// 点光源用
		PointLight::ConstBufferData pointLights[c_pointLightNum];
	};

private: // 静的メンバ変数
	//デバイス
	static ID3D12Device* dev;

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static bool StaticInitialize(ID3D12Device* dev);

	/// <summary>
	/// 生成
	/// </summary>
	static LightGroup* Create();

private: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// ライトのベクトル
	XMVECTOR lightDir = { 1, 0, 0, 0 };
	// 色
	XMFLOAT3 lightColor = { 1, 1, 1 };
	// ダーティーフラグ
	bool dirty = false;
	// ダーティーフラグが立ったか
	bool isDirty = true;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッフに転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	/// <summary>
	/// ライト更新の成否
	/// </summary>
	/// <returns>成否</returns>
	bool GetDirty() { return isDirty; };
};