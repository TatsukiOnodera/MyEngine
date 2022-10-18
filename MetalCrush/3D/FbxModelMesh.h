#pragma once
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

class FbxModelMesh
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //定数
	//ボーンインデックスの最大値
	static const int MAX_BONE_INDICES = 4;

public: //サブクラス
	//頂点データ構造体
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos; //XYZ座標
		DirectX::XMFLOAT3 normal; //法線ベクトル
		DirectX::XMFLOAT2 uv; //UV座標
		UINT boneIndex[MAX_BONE_INDICES]; //ボーン番号
		float boneWeight[MAX_BONE_INDICES]; //ボーン重み
	};

public: //メンバ変数
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices;
	//頂点インデックス配列
	std::vector<unsigned short> indices;

public: //メンバ関数
	/// <summary>
	/// バッファ生成
	/// </summary>
	void CreateBuffers(ID3D12Device* dev);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);
};