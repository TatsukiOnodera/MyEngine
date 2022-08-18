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
#include "Bullet.h"
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
	//Audio* audio = nullptr;
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
	
	//OBJオブジェクト
	std::unique_ptr<Enemy> enemy = nullptr;
	std::array<std::unique_ptr<Object3d>, END> defaultWall = {};
	std::vector< std::unique_ptr<Bullet>> enemyBullet;
	std::vector<std::unique_ptr<Bullet>> playerBullet;
	//FBXオブジェクト
	std::unique_ptr<Player> player = nullptr;

private: //メンバ変数
	//ショットの間隔
	int intervalTime = 0;

public: //メンバ関数
	~GamePlayScene() override;

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
