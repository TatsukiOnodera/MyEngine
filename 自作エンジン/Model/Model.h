#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

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
	static ID3D12Device* device;

	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;

private: //メンバ変数
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;

	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};

	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices;

	// 頂点インデックス配列
	std::vector<unsigned short> indices;

	//マテリアル
	Material material;

	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;

	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;

	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;

	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;

public: //静的メンバ関数
	//静的初期化
	static void StaticInitialize(ID3D12Device* device);

	//モデル生成
	static Model* CreateModel(const std::string& modelName = "defaultTexture");

public: //メンバ関数
	//マテリアルの取得
	Material GetMaterial() { return material; }

	//デスクリプタヒープの初期化
	void InitializeDescriptorHeap();

	//テクスチャ読み込み
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	//モデル作成
	void InitializeModel(const std::string& modelName);

	//マテリアルの読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	//初期化・更新・描画
	void Initialize();
	void Update(Material material);
	void Draw(ID3D12GraphicsCommandList* cmdList);
};