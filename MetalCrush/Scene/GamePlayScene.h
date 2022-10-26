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
#include "Bullet.h"

#include <Windows.h>
#include <DirectXMath.h>
#include <memory>
#include <array>

class GamePlayScene : public BaseScene
{
public: // エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 定数
	// デバッグテキスト用テクスチャの番号
	const int fontNumber = 0;

	// シェーダーの種類
	enum ShadersType
	{
		ADS, TOON, MONO, BLEND, SPECULAR
	};

	// 壁の配置
	enum WALLNUMBER
	{
		FRONT, BACK, RIGHT, LEFT, UP, DOWN, END
	};

private: // メモリ置き場
	// DIrectXコマンド
	DirectXCommon* dx_cmd = nullptr;
	// 操作系
	Input* input = nullptr;
	// オーディオ
	//Audio* audio = nullptr;
	// カメラ
	Camera* camera = nullptr;
	// デバッグテキスト
	DebugText debugText;

private: // インスタンス
	// ライト
	std::unique_ptr<Light> light = nullptr;

	// スプライト
	std::unique_ptr<Sprite> backScreen = nullptr;
	std::unique_ptr<Sprite> reticle = nullptr;
	std::unique_ptr<Sprite> HP = nullptr;

	// OBJオブジェクト
	std::unique_ptr<Object3d> ground = nullptr;
	std::vector<std::unique_ptr<Enemy>> enemy;

	// FBXオブジェクト
	std::unique_ptr<Player> player = nullptr;

private: // メンバ変数
	// 標的の番号
	int targetNum = 0;

public: // メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
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
	const float Length(XMFLOAT3 pos1, XMFLOAT3 pos2 = { 0, 0, 0 });

	/// <summary>
	/// 衝突判定
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// プレイヤーとエネミーの衝突判定	
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	/// <param name="enemyPos">エネミーの座標</param>
	void CheckPlayer2Enemy(const XMFLOAT3& playerPos, const XMFLOAT3* enemyPos);

	/// <summary>
	/// プレイヤーの弾とエネミーの衝突判定
	/// </summary>
	/// <param name="playerBullets">プレイヤーの弾</param>
	/// <param name="enemyPos">エネミーの座標</param>
	void CheckPlayerBullets2Enemy(const std::vector<std::unique_ptr<Bullet>>& playerBullets, const XMFLOAT3* enemyPos);

	/// <summary>
	/// プレイヤーとエネミーの弾の衝突判定
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void CheckPlayer2EnemyBullets(const XMFLOAT3& playerPos);

	/// <summary>
	/// プレイヤーと壁の衝突判定
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void CheckPlayer2Wall(XMFLOAT3& playerPos);
	
	/// <summary>
	/// エネミーと壁の衝突判定
	/// </summary>
	/// <param name="playerPos">エネミーの座標</param>
	void CheckEnemy2Wall(XMFLOAT3* enemyPos);

	/// <summary>
	/// プレイヤーの弾と壁の衝突判定
	/// </summary>
	/// <param name="playerBullets">プレイヤーの弾</param>
	void CheckPlayerBullets2Wall(const std::vector<std::unique_ptr<Bullet>>& playerBullets);
	
	/// <summary>
	/// エネミーの弾と壁の衝突判定
	/// </summary>
	/// <param name="enemyBullets">エネミーの弾</param>
	void CheckEnemyBullets2Wall();
};
