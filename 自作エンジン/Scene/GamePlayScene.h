#pragma once
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Audio.h"
#include "DebugText.h"
#include "ParticleManager.h"
#include "Collision.h"
#include "Camera.h"
#include "DirectXCommon.h"
#include "BaseScene.h"
#include "Light.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <memory>

class GamePlayScene : public BaseScene
{
public: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //定数
	const int fontNumber = 0;

private: //メモリ置き場
	//DIrectXCommon
	DirectXCommon* dx_cmd = nullptr;

	//操作系
	Input* input = nullptr;

	//オーディオ
	Audio* audio = nullptr;

	//カメラ
	Camera* camera = nullptr;

	//デバッグテキスト
	DebugText debugText;

private: //オブジェクトのメモリ
	//ライト
	std::unique_ptr<Light> light = nullptr;

	//パーティクル
	std::unique_ptr<ParticleManager> particle = nullptr;

	//スプライト
	

	//OBJオブジェクト
	std::unique_ptr<Object3d> ball = nullptr;
	std::unique_ptr<Object3d> line = nullptr;

	//FBXオブジェクト


private: //メンバ変数
	XMFLOAT3 start = {};
	XMFLOAT3 end = {};
	float r = 0;

public: //メンバ関数
	~GamePlayScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 変数初期化
	/// </summary>
	void ResetVariable();

	/// <summary>
	/// 背景スプライト描画
	/// </summary>
	void DrawBackSprite(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// パーティクル描画
	/// </summary>
	void DrawParticle(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// デバッグテキスト描画
	/// </summary>
	void DrawDebugText(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// ベクトルの長さ
	/// </summary>
	/// <param name="vec">ベクトル</param>
	float CalculationVectorLength(XMFLOAT3 vec);
};