#pragma once
#include <DirectXMath.h>

struct CircleShadow
{
private: // エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 定数バッファ用
	struct ConstBufferData
	{
		XMVECTOR dir;
		XMFLOAT3 casterPos;
		float distanceCasterLight;
		XMFLOAT3 atten;
		float pad1;
		XMFLOAT2  factorAngleCos;
		unsigned int active;
		float pad2;
	};

private: // メンバ変数
	// ベクトル
	XMVECTOR m_dir = { 1, 0, 0, 0 };
	// キャスターとライトの距離
	float m_distanceCasterLight = 100.0f;
	// キャスター座標
	XMFLOAT3 m_casterPos = { 0, 0, 0 };
	// 距離減衰係数
	XMFLOAT3 m_atten = { 0.5f, 0.6f, 0.0f };
	// 減衰角度（開始角度、終了角度）
	XMFLOAT2 m_factorAngleCos = { 0.2f, 0.5f };
	// 有効フラグ
	bool m_active = false;

public: // アクセッサ
	/// <summary>
	/// ベクトルを取得
	/// </summary>
	/// <returns>ベクトル</returns>
	inline XMVECTOR GetDir() { return m_dir; }

	/// <summary>
	/// ベクトルをセット
	/// </summary>
	/// <param name="dir">ベクトル</param>
	inline void SetDir(const XMVECTOR& dir)
	{
		m_dir = DirectX::XMVector3Normalize(dir);
	}

	/// <summary>
	/// キャスター座標を取得
	/// </summary>
	/// <returns>キャスター座標</returns>
	inline XMFLOAT3 GetCasterPos() { return m_casterPos; }

	/// <summary>
	/// キャスター座標をセット
	/// </summary>
	/// <param name="casterPos">キャスター座標</param>
	inline void SetCasterPos(const XMFLOAT3& casterPos)
	{
		m_casterPos = casterPos;
	}

	/// <summary>
	/// キャスターとライトの距離を取得
	/// </summary>
	/// <returns>キャスターとライトの距離</returns>
	inline float GetDistanceCasterLight() { return m_distanceCasterLight; }

	/// <summary>
	/// キャスターとライトの距離をセット
	/// </summary>
	/// <param name="distanceCasterLight">キャスターとライトの距離</param>
	inline void SetDistanceCasterLight(float distanceCasterLight)
	{
		m_distanceCasterLight = distanceCasterLight;
	}

	/// <summary>
	/// 距離減衰係数を取得
	/// </summary>
	/// <returns>距離減衰係数</returns>
	inline const XMFLOAT3& GetAtten() { return m_atten; }

	/// <summary>
	/// 距離減衰係数をセット
	/// </summary>
	/// <param name="atten">距離減衰係数</param>
	inline void SetAtten(const XMFLOAT3& atten)
	{
		m_atten = atten;
	}

	/// <summary>
	/// 距離減衰係数を取得
	/// </summary>
	/// <returns>距離減衰係数</returns>
	inline const XMFLOAT2& GetFactorAngleCos() { return m_factorAngleCos; }

	/// <summary>
	/// ライト距離減衰係数をセット
	/// </summary>
	/// <param name="FactorAngleCos">減衰角度</param>
	inline void SetFactorAngleCos(const XMFLOAT2& factorAngleCos)
	{
		m_factorAngleCos.x = cosf(DirectX::XMConvertToRadians(factorAngleCos.x));
		m_factorAngleCos.y = cosf(DirectX::XMConvertToRadians(factorAngleCos.y));
	}

	/// <summary>
	/// 有効フラグを取得
	/// </summary>
	/// <returns>有効フラグ</returns>
	inline bool GetActive() { return m_active; };

	/// <summary>
	/// 有効フラグをセット
	/// </summary>
	/// <param name="active">有効フラグ</param>
	inline void SetActive(const bool active)
	{
		m_active = active;
	}
};