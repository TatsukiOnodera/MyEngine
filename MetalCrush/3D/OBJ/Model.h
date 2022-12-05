#pragma once
#include "PipelineManager.h"
#include "Mesh.h"

#include <string>
#include <vector>

class Model
{
private: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private: // 定数
	// 基本ディレクトリ
	static const std::string baseDirectory;

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* s_dev;
	// デスクリプタサイズ
	static UINT s_descriptorHandleIncrementSize;

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	static void StaticInitialize(ID3D12Device* dev);

	/// <summary>
	/// モデル生成
	/// </summary>
	/// <param name="modelName">モデルのパス</param>
	/// <param name="smooting">スムーズシェーディングの有無</param>
	/// <returns>モデル</returns>
	static Model* Create(const std::string& modelName, const bool smoothing = false);

private: // メンバ変数
	// グラフィックスパイプライン
	std::unique_ptr<PipelineManager> m_graphicsPipeline = nullptr;
	// 名前
	std::string m_name;
	// メッシュコンテナ
	std::vector<std::unique_ptr<Mesh>> m_meshes;
	// マテリアルコンテナ
	std::unordered_map<std::string, std::unique_ptr<Material>> m_materials;
	// デフォルトマテリアル
	Material* defaultMaterial = nullptr;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> m_descHeap;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Model();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	~Model();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="modelName">モデルのパス</param>
	/// <param name="smooting">スムーズシェーディングの有無</param>
	void Initialize(const std::string& modelName, const bool smoothing);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="filename">ファイル名</param>
	void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// マテリアル登録
	/// </summary>
	/// <param name="material">マテリアル</param>
	inline void AddMaterial(Material* material)
	{
		m_materials.emplace(material->name, material);
	}

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTextures();

public: // アクセッサ
	/// <summary>
	/// メッシュ配列取得
	/// </summary>
	/// <returns>メッシュ配列</returns>
	inline const std::vector<std::unique_ptr<Mesh>>& GetMeshes() { return m_meshes; }

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
};