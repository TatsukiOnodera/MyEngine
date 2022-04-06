#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "Model.h"
#include "Camera.h"
#include "Light.h"

class Object3d
{
public: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //静的変数;
	// デバイス
	static ID3D12Device* dev;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate; 
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	//カメラクラス
	static Camera *camera;
	//ライトクラス
	static Light* light;

public: //静的関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static bool StaticInitialize(ID3D12Device* device, Camera* camera, int window_width, int window_height);

	/// <summary>
	/// グラフィックパイプライン生成
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
	/// オブジェクト生成
	/// </summary>
	static Object3d* Create(const std::string& modelName = "defaultTexture", bool smooting = false);

	/// <summary>
	/// カメラセット
	/// </summary>
	static Camera* GetCamera() { return camera; };

	/// <summary>
	/// ライトセット
	/// </summary>
	static void SetLight(Light* light) { Object3d::light = light; }

public: //サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX viewproj; //ビュープロジェクション行列
		XMMATRIX world; //ワールド行列
		XMFLOAT3 cameraPos; //カメラ座標(ワールド座標)
		XMFLOAT4 color; //色(RGBA)
	};

private: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//平行移動
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	//スケール
	XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
	//回転
	XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
	//色(RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//ワールド行列
	XMMATRIX matWorld;
	//モデルデータ
	Model* model = nullptr;
	// ビルボード
	bool isBillboard = false;

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
	/// 色
	/// </summary>
	XMFLOAT4 GetColor() { return color; }
	void SetColor(XMFLOAT4 color);

	/// <summary>
	/// モデルデータセット
	/// </summary>
	void SetModel(Model* model) { this->model = model; }

	/// <summary>
	/// ビルボードセット
	/// </summary>
	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }
};