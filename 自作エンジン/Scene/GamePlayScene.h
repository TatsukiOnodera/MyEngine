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

#include "Player.h"
#include "Enemy.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <memory>
#include <array>

class GamePlayScene : public BaseScene
{
public: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

<<<<<<< HEAD
private: // 定数
	// デバッグテキスト用テクスチャの番号
	const int fontNumber = 0;
	// シェーダーの種類
	enum ShadersType
	{
		ADS, TOON, MONO, BLEND, SPECULAR
	};
	// 壁の配置
=======
private: //定数
	//デバッグテキスト用テクスチャの番号
	const int fontNumber = 0;

	//壁の配置
>>>>>>> parent of 00f20c5... no message
	enum WALLNUMBER
	{
		FRONT, BACK, RIGHT, LEFT, UP, DOWN, END
	};

<<<<<<< HEAD
private: // メモリ置き場
	// DIrectXCommon
=======
private: //メモリ置き場
	//DIrectXCommon
>>>>>>> parent of 00f20c5... no message
	DirectXCommon* dx_cmd = nullptr;
	// 操作系
	Input* input = nullptr;
	// オーディオ
	//Audio* audio = nullptr;
	// カメラ
	Camera* camera = nullptr;
	// デバッグテキスト
	DebugText debugText;

<<<<<<< HEAD
private: // インスタンス
	// ライト
=======
private: //インスタンス
	//ライト
>>>>>>> parent of 00f20c5... no message
	std::unique_ptr<Light> light = nullptr;
	// パーティクル
	std::unique_ptr<ParticleManager> particle = nullptr;
<<<<<<< HEAD
	// スプライト
	std::unique_ptr<Sprite> sight = nullptr;
	// OBJオブジェクト
	std::array<std::unique_ptr<Object3d>, END> defaultWall = {};
	std::vector<std::unique_ptr<Enemy>> enemy;
	// FBXオブジェクト
	std::unique_ptr<Player> player = nullptr;

private: // メンバ変数
	//標的番号
	int targetNum;
	//標的リスト
	std::vector<int> targetList;
	//リスト番号
	int listNum;

public: // メンバ関数
	~GamePlayScene() override;
=======

	//スプライト
	std::unique_ptr<Sprite> demo_back = nullptr;

	//OBJオブジェクト
	std::array<std::unique_ptr<Object3d>, END> defaultWall;
	
	//FBXオブジェクト
	std::unique_ptr<FbxObject3d> fbxObject = nullptr;

private: //メンバ変数
	


public: //メンバ関数
	~GamePlayScene();
>>>>>>> parent of 00f20c5... no message

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
	void InitializeVariable();

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

	/// <summary>
	/// 始点から終点への距離
	/// </summary>
	/// <param name="pos1">終点</param>
	/// <param name="pos2">始点</param>
	/// <returns>二点間の距離</returns>
	const float Length(XMFLOAT3 pos1, XMFLOAT3 pos2);
};
