#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>

class Material
{
private: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT3 ambient; // アンビエント係数
		float pad1; // パディング
		XMFLOAT3 diffuse; // ディフューズ係数
		float pad2; // パディング
		XMFLOAT3 specular; // スペキュラー係数
		float alpha; // アルファ
	};

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* s_dev;

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* dev);

	/// <summary>
	/// マテリアル生成
	/// </summary>
	/// <returns>マテリアル</returns>
	static Material* Create();

public: // マテリアルデータ
	// マテリアル名
	std::string name;
	// アンビエント影響度
	XMFLOAT3 ambient;	
	// ディフューズ影響度
	XMFLOAT3 diffuse;
	// スペキュラー影響度
	XMFLOAT3 specular;
	// アルファ
	float alpha;
	// テクスチャファイル名
	std::string textureFilename;

private: // メンバ変数
	// テクスチャバッファ
	ComPtr<ID3D12Resource> m_texbuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> m_constBuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpuDescHandleSRV;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Material();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 定数バッファの生成
	/// </summary>
	void CreateConstBuffer();

	/// テクスチャ読み込み
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="cpuDescHandleSRV">シェーダリソースビューのハンドル(CPU)</param>
	/// <param name="gpuDescHandleSRV">シェーダリソースビューのハンドル(CPU)</param>
	void LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV);

public: // アクセッサ
	/// <summary>
	/// 定数バッファの取得
	/// </summary>
	/// <returns>定数バッファ</returns>
	inline ID3D12Resource* GetConstantBuffer() { return m_constBuff.Get(); }

	/// <summary>
	/// シェーダリソースビューのハンドル（CPU）取得
	/// </summary>
	/// <returns>シェーダリソースビューのハンドル（CPU）</returns>
	inline const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCPUHandle() { return m_cpuDescHandleSRV; }

	/// <summary>
	/// シェーダリソースビューのハンドル（GPU）取得
	/// </summary>
	/// <returns>シェーダリソースビューのハンドル（GPU）</returns>
	inline const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGPUHandle() { return m_gpuDescHandleSRV; }
};