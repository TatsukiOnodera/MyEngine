#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Audio.h"
#include "DebugText.h"
#include "ParticleManager.h"
#include "Collision.h"
#include "Camera.h"
#include "DirectXCommon.h"

class GamePlayScene
{
public: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //メモリ置き場
	DirectXCommon* dx_cmd = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	Camera* camera = nullptr;

private: //メンバ変数
	//デバッグテキスト
	DebugText debugText;
	const int fontNumber = 0;

	//パーティクル
	ParticleManager* particle = nullptr;

	//スプライト
	Sprite* demo_back = nullptr;
	Sprite* demo_spr = nullptr;

	//オブジェクト
	Object3d* chr = nullptr;
	Object3d* obj = nullptr;

public: //メンバ関数
	GamePlayScene();
	~GamePlayScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dx_cmd, Input* input, Audio* sound, Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 背景スプライト描画
	/// </summary>
	void DrawBackSprite();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクル描画
	/// </summary>
	void DrawParticles();

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// デバッグテキスト描画
	/// </summary>
	void DrawDebugText();

	/// <summary>
	/// 変数初期化
	/// </summary>
	void ResetVariable();
};