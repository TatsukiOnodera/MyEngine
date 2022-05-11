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
	/// <summary>
	/// 頂点座標読み取り
	/// </summary>
	/// <param name="fbxModel">読み込み先モデルオブジェクト</param>
	/// <param name="fbxMesh">解析対象のメッシュ</param>
	void ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// 面情報読み取り
	/// </summary>
	/// <param name="fbxModel">読み込み先モデルオブジェクト</param>
	/// <param name="fbxMesh">解析対象のメッシュ</param>
	void ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// マテリアル読み取り
	/// </summary>
	/// <param name="fbxModel">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode);

	/// <summary>
	/// テクスチャ読み取り
	/// </summary>
	/// <param name="fbxModel">読み込み先モデルオブジェクト</param>
	/// <param name="fullpath">アドレス</param>
	void LoadTexture(FbxModel* fbxModel, const std::string& fullpath);

	/// <summary>
	/// ディレクトリを含んだファイルパスからファイル名を抽出
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <returns>ファイル名</returns>
	std::string ExtractFileName(const std::string& path);

	/// <summary>
	/// スキニング情報の読み取り
	/// </summary>
	/// <param name="fbxModel">読み込み先モデルオブジェクト</param>
	/// <param name="fbxMesh">解析対象のメッシュ</param>
	void ParseSkin(FbxModel* fbxModel, FbxMesh* fbxMesh);

public: //静的メンバ関数
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// FBX行列をXMMatrixに変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">元になるFBX行列</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

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
	FbxModel* LoadModelFromFile(const string& modelName);

	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="fbxModel">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="fbxModel">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode);
};