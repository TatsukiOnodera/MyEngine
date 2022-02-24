#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "Model.h"
#include "Camera.h"

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

public: //静的関数
	//静的初期化
	static bool StaticInitialize(ID3D12Device* device, Camera* camera, int window_width, int window_height);

	//グラフィックパイプラインの生成
	static void CreateGraphicsPipeline();

	//描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	//描画後処理
	static void PostDraw();

	//オブジェクト生成
	static Object3d* Create(const std::string& modelName);

public: //サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat; //行列
		XMFLOAT4 color; //色(RGBA)
	};

private: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;

	//平行移動
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f }; //座標を変数として持つ

	//スケール
	XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f }; //座標を変数として持つ

	//回転
	XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f }; //座標を変数として持つ

	//色(RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };

	//ワールド行列
	XMMATRIX matWorld;

	//モデルデータ
	Model* model = nullptr;

public: //アクセッサ
	//座標
	XMFLOAT3 GetPosition() { return position; }
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	//回転
	XMFLOAT3 GetRotation() { return rotation; }
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	//スケール
	XMFLOAT3 GetScale() { return scale; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	//色
	XMFLOAT4 GetColor() { return color; }
	void SetColor(XMFLOAT4 color) { this->color = color; }

	//モデルデータセット
	void SetModel(Model* model) { this->model = model; }

public: //メンバ関数
	//初期化・更新・描画
	void Initialize();
	void Update();
	void Draw();
};