#pragma once
#include "FbxModel.h"
#include "FbxLoader.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FbxObject3d
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

public: //定数
	//ボーンの最大数
	static const int MAX_BONES = 32;

public: //サブクラス
	//定数バッファ構造体
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj; //ビュープロジェクション行列
		XMMATRIX world; //ワールド行列
		XMFLOAT3 cameraPos; //カメラ座標（ワールド座標）
	};

	//定数バッファ用データ構造体
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

private: //静的メンバ変数
	//デバイス
	static ID3D12Device* dev;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//カメラ
	static Camera* camera;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	//FBX読み込み
	static FbxLoader* fbxLoader;

public: //静的メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	~FbxObject3d();

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxObject3d* GetInstance();

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
	/// 描画前処理
	/// </summary>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// FBX作成
	/// </summary>
	/// <param name="modelName">FBXモデル名</param>
	/// <returns></returns>
	static FbxObject3d* CreateFBXObject(const string& modelName);

private: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBufferTransform;
	//定数バッファ（スキン）
	ComPtr<ID3D12Resource> constBufferSkin;
	//ローカルスケール
	XMFLOAT3 scale = { 1, 1, 1 };
	//ローカル回転角
	XMFLOAT3 rotation = { 0, 0, 0 };
	//ローカル座標
	XMFLOAT3 position = { 0, 0, 0 };
	//ローカルワールド行列
	XMMATRIX matWorld;
	//FBXモデル
	FbxModel* fbxModel = nullptr;
	//1フレーム
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時刻（アニメーション）
	FbxTime currentTime;
	//アニメーションフラグ
	bool isPlay = false;
	//アニメーションループフラグ
	bool isLoop = false;

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
	void Draw();

	/// <summary>
	/// アニメーション開始
	/// </summary>
	/// <param name="number">アニメーションの番号</param>
	/// <param name="loop">ループの成否</param>
	void PlayAnimation(const int number = 0, const bool loop = false);

	/// <summary>
	/// アニメーションをリセット
	/// </summary>
	void ResetAnimation();

public: //アクセッサ
	/// <summary>
	/// 座標
	/// </summary>
	XMFLOAT3 GetPosition() { return position; }
	void SetPosition(XMFLOAT3 position);

	/// <summary>
	/// 回転
	/// </summary>
	XMFLOAT3 GetRotation() { return rotation; }
	void SetRotation(XMFLOAT3 rotation);

	/// <summary>
	/// スケール
	/// </summary>
	XMFLOAT3 GetScale() { return scale; }
	void SetScale(XMFLOAT3 scale);

	/// <summary>
	/// アニメーション中
	/// </summary>
	bool GetIsPlay() { return isPlay; }

	/// <summary>
	/// ループ
	/// </summary>
	bool GetLoop() { return isLoop; }
	void SetLoop(bool loop);

	/// <summary>
	/// FBXモデルのセット
	/// </summary>
	/// <param name="model">FBXモデル</param>
	void SetFBXModel(FbxModel* fbxModel) { this->fbxModel = fbxModel; }
};