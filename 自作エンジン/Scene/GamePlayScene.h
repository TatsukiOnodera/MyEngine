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
#include <array>

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
	//デバッグテキスト用テクスチャの番号
	const int fontNumber = 0;
	//シェーダーの種類
	enum ShadersType
	{
		ADS, TOON, MONO, BLEND, SPECULAR
	};
	//壁の配置
	enum WALLNUMBER
	{
		FRONT, BACK, RIGHT, LEFT, UP, DOWN, END
	};

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

private: //インスタンス
	//ライト
	std::unique_ptr<Light> light = nullptr;
	//パーティクル
	std::unique_ptr<ParticleManager> particle = nullptr;
	//スプライト
	std::unique_ptr<Sprite> demo_back = nullptr;
	//OBJオブジェクト
	std::unique_ptr<Object3d> obj = nullptr;
	std::array<std::unique_ptr<Object3d>, END> defaultWall;
	std::vector<std::unique_ptr<Object3d>> bullet;
	std::unique_ptr<Object3d> enemy = nullptr;
	//FBXオブジェクト
	std::unique_ptr<FbxObject3d> fbxObject = nullptr;

private: //メンバ変数
	//加速する
	bool isDash;
	//初期加速値
	float add0;

	//Bulletの生死
	std::vector<bool> bulletAlive;
	//BUlletのベクトル
	std::vector<XMFLOAT3> bulletVec;
	//ショットの間隔
	int bulletTime;

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
	void ResetParameter();

	/// <summary>
	/// 背景スプライト描画
	/// </summary>
	void DrawBackSprite(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画
	/// </summary>
	void DrawObjects(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// エフェクト描画
	/// </summary>
	void DrawEffect(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// デバッグテキスト描画
	/// </summary>
	void DrawDebugText(ID3D12GraphicsCommandList* cmdList);
};