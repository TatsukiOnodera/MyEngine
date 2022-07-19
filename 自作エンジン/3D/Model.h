#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <unordered_map>

#include "PipelineManager.h"

class Model
{
private: //エイリアス
	//Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //定数
	static const int maxTexNum = 3;

public: //サブクラス
	//頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT3 ambient; //アンビエント係数
		float pad1; //パディング
		XMFLOAT3 diffuse; //ディフューズ係数
		float pad2; //パディング
		XMFLOAT3 specular; //スペキュラー係数
		float alpha; //アルファ
	};
	//マテリアル
	struct Material
	{
		std::string name; //マテリアル名
		XMFLOAT3 ambient; //アンビエント影響度
		XMFLOAT3 diffuse; //ディフューズ影響度
		XMFLOAT3 specular; //スペキュラー影響度
		float alpha; //アルファ
		std::string textureFilename = "defaultTexture"; //テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.5, 0.5, 0.5 };
			diffuse = { 0.0, 0.0, 0.0 };
			specular = { 0.0, 0.0, 0.0 };
			alpha = 1.0;
		}
	};

private: //静的メンバ変数
	// デバイス
	static ID3D12Device* s_dev;
	// デスクリプタサイズ
	static UINT s_descriptorHandleIncrementSize;

public: //静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize(ID3D12Device* dev);

	/// <summary>
	/// モデル生成
	/// </summary>
	static Model* CreateModel(const std::string& modelName, bool smooting);

private: //メンバ変数
	//グラフィックスパイプライン
	std::unique_ptr<PipelineManager> m_graphicsPipeline = nullptr;
	// 頂点バッファ
	ComPtr<ID3D12Resource> m_vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> m_indexBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW m_vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW m_ibView = {};
	// 頂点データ配列s
	std::vector<VertexPosNormalUv> m_vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> m_indices;
	//マテリアル
	Material m_material;
	//テクスチャ名
	std::vector<std::string> textureName;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> m_texbuff[maxTexNum];
	//定数バッファ
	ComPtr<ID3D12Resource> m_constBuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuDescHandleSRV[maxTexNum];
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpuDescHandleSRV[maxTexNum];
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> m_descHeap;
	//頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> m_smoothData;

public: //メンバ関数
	/// <summary>
	/// マテリアルの取得
	/// </summary>
	Material GetMaterial() { return m_material; }

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	void CreateGraphicsPipeline();

	/// <summary>
	/// テクスチャ名読み込み
	/// </summary>
	void LoadTextureName(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture();

	/// <summary>
	/// モデル作成
	/// </summary>
	void InitializeModel(const std::string& modelName, bool smooting);

	/// <summary>
	/// エッジ平滑化データの追加
	/// </summary>
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);

	/// <summary>
	/// 平滑化された頂点法線の計算
	/// </summary>
	void CalculateSmoothedVertexNormals();

	/// <summary>
	/// マテリアルの読み込み
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

public: //アクセッサ
	/// <summary>
	/// パイプラインステートを取得
	/// </summary>
	/// <returns>パイプラインステート</returns>
	ID3D12PipelineState* GetPipelineState() { return m_graphicsPipeline->GetPipelineState(); }

	/// <summary>
	/// ルートシグネチャを取得
	/// </summary>
	/// <returns>ルートシグネチャ</returns>
	ID3D12RootSignature* GetRootSignature() { return m_graphicsPipeline->GetRootSignature(); }

	/// <summary>
	/// グラフィックスパイプラインセット
	/// </summary>
	/// <param name="shaderType">シェーダーの種類</param>
	void SetGraphicsPipeline(const int shaderType);

	/// <summary>
	/// サブテクスチャのセット
	/// </summary>
	/// <param name="directoryPath">ダイレクトパス</param>
	/// <param name="filename">ファイル名</param>
	void SetSubTexture(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// マスクテクスチャのセット
	/// </summary>
	/// <param name="directoryPath">ダイレクトパス</param>
	/// <param name="filename">ファイル名</param>
	void SetMaskTexture(const std::string& directoryPath, const std::string& filename);
};