#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Material.h"
#include <vector>
#include <unordered_map>

class Mesh
{
private: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

private: // 静的メンバ変数
// デバイス
	static ID3D12Device* s_dev;

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	static void StaticInitialize(ID3D12Device* dev);

private: // メンバ変数
	// 名前
	std::string m_name;
	// 頂点バッファ
	ComPtr<ID3D12Resource> m_vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> m_indexBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW m_vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW m_ibView = {};
	// 頂点データ配列
	std::vector<VertexPosNormalUv> m_vertices;
	// 頂点インデックス配列
	std::vector<uint32_t> m_indices;
	// 頂点法線スムージング用データ
	std::unordered_map<uint32_t, std::vector<uint32_t>> m_smoothData;
	// マテリアル
	Material* m_material = nullptr;

public: // メンバ関数
	/// <summary>
	/// バッファの生成
	/// </summary>
	void CreateBuffers();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">命令発行先コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// エッジ平滑化データの追加
	/// </summary>
	/// <param name="indexPosition">頂点位置</param>
	/// <param name="indexVertex">頂点座標</param>
	inline void AddSmoothData(uint32_t indexPosition, uint32_t indexVertex)
	{
		m_smoothData[indexPosition].emplace_back(indexVertex);
	}

	/// <summary>
	/// 平滑化された頂点法線の計算
	/// </summary>
	void CalculateSmoothedVertexNormals();

public: // アクセッサ
	/// <summary>
	/// 名前を取得
	/// </summary>
	/// <returns>名前</returns>
	inline const std::string& GetName() { return m_name; }

	/// <summary>
	/// 名前をセット
	/// </summary>
	/// <param name="name">名前</param>
	inline void SetName(const std::string& name)
	{
		m_name = name;
	}

	/// <summary>
	/// 頂点データの追加
	/// </summary>
	/// <param name="vertex">頂点データ</param>
	inline void AddVertex(const VertexPosNormalUv& vertex)
	{
		m_vertices.emplace_back(vertex);
	}

	/// <summary>
	/// 頂点インデックスの追加
	/// </summary>
	/// <param name="index">インデックス</param>
	inline void AddIndex(uint32_t index)
	{
		m_indices.emplace_back(index);
	}

	/// <summary>
	/// 頂点データの数を取得
	/// </summary>
	/// <returns>頂点データの数</returns>
	inline size_t GetVertexCount() { return m_vertices.size(); }

	/// <summary>
	/// 指定の頂点データ取得
	/// </summary>
	/// <param name="index">指定の番号</param>
	/// <returns>頂点データ</returns>
	inline const VertexPosNormalUv& GetVertices(uint32_t index) { return m_vertices[index]; }

	/// <summary>
	/// マテリアルの取得
	/// </summary>
	/// <returns>マテリアル</returns>
	inline Material* GetMaterial() { return m_material; }

	/// <summary>
	/// マテリアルの割り当て
	/// </summary>
	/// <param name="material">マテリアル</param>
	inline void SetMaterial(Material* material)
	{
		m_material = material;
	}

	/// <summary>
	/// 頂点バッファ取得
	/// </summary>
	/// <returns>頂点バッファ</returns>
	inline const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return m_vbView; }

	/// <summary>
	/// インデックスバッファ取得
	/// </summary>
	/// <returns>インデックスバッファ</returns>
	inline const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return m_ibView; }
};
