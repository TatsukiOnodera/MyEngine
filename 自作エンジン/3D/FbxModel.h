#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

//ノード構造体
struct Node
{
	//名前
	std::string name;
	//ローカルスケール
	DirectX::XMVECTOR scaling = { 1, 1, 1, 0 };
	//ローカル回転角
	DirectX::XMVECTOR rotation = { 0, 0, 0, 0 };
	//ローカル移動
	DirectX::XMVECTOR translation = { 0, 0, 0, 1 };
	//ローカル変形行列
	DirectX::XMMATRIX transform;
	//グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	//親ノード
	Node* parent = nullptr;
};

class FbxModel
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::を省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public: //メモリ
	//フレンドクラスを作る
	friend class FbxLoader;

private: //定数
//ボーンインデックスの最大値
	static const int MAX_BONE_INDICES = 4;

public: //サブクラス
	//頂点データ構造体
	struct VertexPosNormalUVSkin
	{
		DirectX::XMFLOAT3 pos; //XYZ座標
		DirectX::XMFLOAT3 normal; //法線ベクトル
		DirectX::XMFLOAT2 uv; //UV座標
		UINT boneIndex[MAX_BONE_INDICES]; //ボーン番号
		float boneWeight[MAX_BONE_INDICES]; //ボーン重み
	};

	//ボーン構造体
	struct Bone
	{
		//名前
		std::string name;
		//初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;
		//クラスター（FBX側のボーン情報）
		FbxCluster* fbxCluster;
		//コンストラクタ
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

private: //モデルデータ用変数
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node>nodes;
	//メッシュを持つノード
	Node* meshNode = nullptr;
	//頂点データ配列
	std::vector<VertexPosNormalUVSkin> vertices;
	//頂点インデックス配列
	std::vector<unsigned short> indices;
	//アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1, 1, 1 };
	//ディフューズ係数
	DirectX::XMFLOAT3 diffuse = { 1, 1, 1 };
	//テクスチャメタデータ
	DirectX::TexMetadata metadata = {};
	//スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};
	//ボーン配列
	std::vector<Bone> bones;
	//FBXシーン
	FbxScene* fbxScene = nullptr;

private: //メンバ変数
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

public: //メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	~FbxModel();

	/// <summary>
	/// バッファ生成
	/// </summary>
	void CreateBuffers(ID3D12Device* dev);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// モデルの変形行列取得
	/// </summary>
	/// <returns>モデルの変形行列</returns>
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

	/// <summary>
	/// ボーン配列の取得
	/// </summary>
	/// <returns>ボーン配列</returns>
	std::vector<Bone>& GetBones() { return bones; }

	/// <summary>
	/// FBXシーンの取得
	/// </summary>
	/// <returns>FBXシーン</returns>
	FbxScene* GetFbxScene() { return fbxScene; }
};