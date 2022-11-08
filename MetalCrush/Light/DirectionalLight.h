#pragma once
#include <DirectXMath.h>

class DirectionalLight
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
		XMVECTOR lightDir;
		XMFLOAT3 lightColor;
		unsigned int active;
	};

private: // メンバ変数
	// ライトのベクトル
	XMVECTOR m_lightDir = { 1, 0, 0, 0 };
	// ライトの色
	XMFLOAT3 m_lightColor = { 1, 1, 1 };
	// 有効フラグ
	bool m_active = false;

public: // アクセッサ
	/// <summary>
	/// ライトのベクトルを取得
	/// </summary>
	/// <returns>ライトのベクトル</returns>
	inline const XMVECTOR& GetLightDir() { return m_lightDir; }

	/// <summary>
	/// ライトのベクトルをセット
	/// </summary>
	/// <param name="lightPos">ライトのベクトル</param>
	inline void SetLightDir(const XMVECTOR& lightDir)
	{
		m_lightDir = lightDir;
	}

	/// <summary>
	/// ライト色を取得
	/// </summary>
	/// <returns>ライト色</returns>
	inline const XMFLOAT3& GetLightColor() { return m_lightColor; }

	/// <summary>
	/// ライト色をセット
	/// </summary>
	/// <param name="lightPos">ライト色</param>
	inline void SetLightColor(const XMFLOAT3& lightColor)
	{
		m_lightColor = lightColor;
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
