#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class LightGroup
{
private: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 定数
	// 平行光源の数
	static const int c_pointLightNum = 3;
	// 点光源の数
	static const int c_dirLightNum = 3;
	// スポットライトの数
	static const int c_spotLightNum = 3;
	// 丸影の数
	static const int c_circleShadowNum = 13;

public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		// 環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		// 平行光源用
		DirectionalLight::ConstBufferData dirLights[c_dirLightNum];
		// 点光源用
		PointLight::ConstBufferData pointLights[c_pointLightNum];
		// スポットライト用
		SpotLight::ConstBufferData spotLights[c_spotLightNum];
		// 丸影用
		CircleShadow::ConstBufferData circleShadows[c_circleShadowNum];
	};

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* s_dev;

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static bool StaticInitialize(ID3D12Device* dev);

	/// <summary>
	/// 生成
	/// </summary>
	static LightGroup* Create();

private: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> m_constBuff;
	// 環境光の色
	XMFLOAT3 m_ambientColor = { 1,1,1 };
	// 平行光源の配列
	DirectionalLight m_dirLights[c_dirLightNum];
	// 点光源の配列
	PointLight m_pointLights[c_pointLightNum];
	// スポットライトの配列
	SpotLight m_spotLights[c_spotLightNum];
	// 丸影の行列
	CircleShadow m_circleShadows[c_circleShadowNum];
	// ダーティーフラグ
	bool m_dirty = false;
	// ダーティーフラグが立ったか
	bool m_isDirty = false;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッフに転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// 標準のライト設定
	/// </summary>
	void DefaultLightSetting();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

public: // アクセッサ
	/// <summary>
	/// 環境光のライト色をセット
	/// </summary>
	/// <param name="color">ライト色</param>
	void SetAmbientColor(const XMFLOAT3& color);

	/// <summary>
	/// 平行光源の有効フラグをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="active">有効フラグ</param>
	void SetDirLightActive(int index, bool active);

	/// <summary>
	/// 平行光源のライト方向をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightDir">ライト方向</param>
	void SetDirLightDir(int index, const XMVECTOR& lightDir);

	/// <summary>
	/// 平行光源のライト色をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightColor">ライト色</param>
	void SetDirLightColor(int index, const XMFLOAT3& lightColor);

	/// <summary>
	/// 点光源の有効フラグをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="active">有効フラグ</param>
	void SetPointLightActive(int index, bool active);

	/// <summary>
	/// 点光源のライト座標をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightPos">ライト座標</param>
	void SetPointLightPos(int index, const XMFLOAT3& lightPos);

	/// <summary>
	/// 点光源のライト色をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightColor">ライト色</param>
	void SetPointLightColor(int index, const XMFLOAT3& lightColor);

	/// <summary>
	/// 点光源のライト距離減衰係数をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightAtten">ライト距離減衰係数</param>
	void SetPointLightAtten(int index, const XMFLOAT3& lightAtten);

	/// <summary>
	/// スポットライトの有効フラグをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="active">有効フラグ</param>
	void SetSpotLightActive(int index, bool active);

	/// <summary>
	/// スポットライトのライト方向をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightDir">ライト方向</param>
	void SetSpotLightDir(int index, const XMVECTOR& lightDir);

	/// <summary>
	/// スポットライトのライト座標をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightPos">ライト座標</param>
	void SetSpotLightPos(int index, const XMFLOAT3& lightPos);

	/// <summary>
	/// スポットライトのライト色をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightColor">ライト色</param>
	void SetSpotLightColor(int index, const XMFLOAT3& lightColor);

	/// <summary>
	/// スポットライトのライト距離減衰係数をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightAtten">ライト距離減衰係数</param>
	void SetSpotLightAtten(int index, const XMFLOAT3& lightAtten);

	/// <summary>
	/// スポットライトのライト減衰角度をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightAtten">ライト減衰角度</param>
	void SetSpotLightFactorAngleCos(int index, const XMFLOAT2& lightFactorAngleCos);

	/// <summary>
	/// 丸影の有効フラグをセット
	/// </summary>
	/// <param name="index">丸影の番号</param>
	/// <param name="active">有効フラグ</param>
	void SetCircleShadowActive(int index, bool active);

	/// <summary>
	/// 丸影のキャスター座標をセット
	/// </summary>
	/// <param name="index">丸影の番号</param>
	/// <param name="casterPos">キャスター座標</param>
	void SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos);

	/// <summary>
	/// 丸影の方向をセット
	/// </summary>
	/// <param name="index">丸影の番号</param>
	/// <param name="lightDir">方向</param>
	void SetCircleShadowDir(int index, const XMVECTOR& lightDir);

	/// <summary>
	/// 丸影のキャスターとライトの距離
	/// </summary>
	/// <param name="index">丸影の番号</param>
	/// <param name="distanceCasterLight">キャスターとライトの距離</param>
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);

	/// <summary>
	/// 丸影の距離減衰係数をセット
	/// </summary>
	/// <param name="index">丸影の番号</param>
	/// <param name="lightAtten">距離減衰係数</param>
	void SetCircleShadowAtten(int index, const XMFLOAT3& lightAtten);

	/// <summary>
	/// 丸影の減衰角度をセット
	/// </summary>
	/// <param name="index">丸影の番号</param>
	/// <param name="lightAtten">減衰角度</param>
	void SetCircleShadowFactorAngleCos(int index, const XMFLOAT2& lightFactorAngleCos);

	/// <summary>
	/// ライト更新の成否
	/// </summary>
	/// <returns>成否</returns>
	inline bool GetDirty() { return m_isDirty; };
};