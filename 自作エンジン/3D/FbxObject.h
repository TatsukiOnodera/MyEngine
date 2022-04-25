#pragma once
#include "FbxModel.h"
#include "Camera.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FbxObject
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using string = std::string;

public: //サブクラス
	//定数バッファ構造体
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj; //ビュープロジェクション行列
		XMMATRIX world; //ワールド行列
		XMFLOAT3 cameraPos; //カメラ座標（ワールド座標）
	};

private: //静的メンバ変数
	//デバイス
	static ID3D12Device* dev;
	//カメラ
	static Camera* camera;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;

public: //静的メンバ関数
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxObject *GetInstance();

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* dev);

	/// <summary>
	/// パイプライン生成
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// FBX作成
	/// </summary>
	/// <param name="modelName">FBXモデル名</param>
	/// <returns></returns>
	static FbxModel* CreateFBXModel(const string& modelName);

protected: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBufferTransform;
	//ローカルスケール
	XMFLOAT3 scale = { 1, 1, 1 };
	//ローカル回転角
	XMFLOAT3 rotation = { 0, 0, 0 };
	//ローカル座標
	XMFLOAT3 position = { 0, 0, 0 };
	//ローカルワールド行列
	XMMATRIX matWorld;
	//FBXモデル
	FbxModel* model = nullptr;
	//FBXローダー
	FbxLoader* loader = nullptr;

public: //メンバ関数
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
	/// FBXモデルのセット
	/// </summary>
	/// <param name="model">FBXモデル</param>
	void SetFBXModel(FbxModel* model) { this->model = model; }

	void SetDevice(ID3D12Device* dev) { this->dev = dev; }
	void SetCamera(Camera* camera) { this->camera = camera; }
};