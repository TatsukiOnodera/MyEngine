#pragma once
#include "FbxModel.h"

#include <fbxsdk.h>
#include <cassert>
#include <string.h>
#include <d3d12.h>
#include <d3dx12.h>

class FbxLoader
{
private: //エイリアス
	//std::を省略
	using string = std::string;

public: //定数
	//モデル格納ルートパス
	static const string baseDirectory;
	//テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;

private: //メンバ変数
//デバイス
	ID3D12Device* dev = nullptr;
	//FBXマネージャ
	FbxManager* fbxManager = nullptr;
	//FBXインポータ
	FbxImporter* fbxImporter = nullptr;

private: //サブ関数
	//頂点座標読み取り
	void ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//面情報読み取り
	void ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//マテリアル読み取り
	void ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode);
	//テクスチャ読み取り
	void LoadTexture(FbxModel* fbxModel, const std::string& fullpath);
	//ディレクトリを含んだファイルパスからファイル名を抽出
	std::string ExtractFileName(const std::string& path);

public: //静的メンバ関数
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">D3D12デバイス</param>
	bool Initialize(ID3D12Device* device);

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	/// <summary>
	/// ファイルからFBXモデルを読み込み
	/// </summary>
	/// <param name="modelName">モデル名</param>
	void LoadModelFromFile(const string& modelName);

	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="fbxModel"></param>
	/// <param name="fbxNode"></param>
	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode);
};