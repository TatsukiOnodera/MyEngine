#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

class Light
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

public: //サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMVECTOR lightv; //ライトのベクトル
		XMFLOAT3 lightcolor; //色
	};

private: //静的メンバ変数
	//デバイス
	static ID3D12Device* dev;

public: //静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static bool StaticInitialize(ID3D12Device *dev);

	/// <summary>
	/// 生成
	/// </summary>
	static Light* Create();

private: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//ライトのベクトル
	XMVECTOR lightDir = {1, 0, 0, 0};
	//色
	XMFLOAT3 lightColor = {1, 1, 1};
	//ダーティーフラグ
	bool dirty = false;

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファに送る
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// ライトの方向セット
	/// </summary>
	void SetLightDir(const XMVECTOR& lightDir);

	/// <summary>
	/// ライトの色セット
	/// </summary>
	void SetLightColor(const XMFLOAT3& lightColor);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList *cmdList, UINT rootParameterIndex);
};