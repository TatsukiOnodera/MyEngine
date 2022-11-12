#pragma once
#include <DirectXMath.h>

struct SpotLight
{
private: // �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// �萔�o�b�t�@�p
	struct ConstBufferData
	{
		XMVECTOR lightDir;
		XMFLOAT3 lightPos;
		float pad1;
		XMFLOAT3 lightColor;
		float pad2;
		XMFLOAT3 lightAtten;
		float pad3;
		XMFLOAT2  lightFactorAngleCos;
		unsigned int active;
		float pad4;
	};

private: // �����o�ϐ�
	// ���C�g�̃x�N�g��
	XMVECTOR m_lightDir = { 1, 0, 0, 0 };
	// ���C�g���W�i���[���h���W�j
	XMFLOAT3 m_lightPos = { 0, 0, 0 };
	// ���C�g�F
	XMFLOAT3 m_lightColor = { 1, 1, 1 };
	// ���C�g���������W��
	XMFLOAT3 m_lightAtten = { 1.0f, 1.0f, 1.0f };
	// ���C�g�����p�x�i�J�n�p�x�A�I���p�x�j
	XMFLOAT2 m_lightFactorAngleCos = { 0.5f, 0.2f };
	// �L���t���O
	bool m_active = false;

public: // �A�N�Z�b�T
	/// <summary>
	/// ���C�g�̃x�N�g�����擾
	/// </summary>
	/// <returns>���C�g�̃x�N�g��</returns>
	inline const XMVECTOR& GetLightDir() { return m_lightDir; }

	/// <summary>
	/// ���C�g�̃x�N�g�����Z�b�g
	/// </summary>
	/// <param name="lightPos">���C�g�̃x�N�g��</param>
	inline void SetLightDir(const XMVECTOR& lightDir)
	{
		m_lightDir = DirectX::XMVector3Normalize(lightDir);
	}

	/// <summary>
	/// ���C�g���W���擾
	/// </summary>
	/// <returns>���C�g���W</returns>
	inline const XMFLOAT3& GetLightPos() { return m_lightPos; }

	/// <summary>
	/// ���C�g���W���Z�b�g
	/// </summary>
	/// <param name="lightPos">���C�g���W</param>
	inline void SetLightPos(const XMFLOAT3& lightPos)
	{
		m_lightPos = lightPos;
	}

	/// <summary>
	/// ���C�g�F���擾
	/// </summary>
	/// <returns>���C�g�F</returns>
	inline const XMFLOAT3& GetLightColor() { return m_lightColor; }

	/// <summary>
	/// ���C�g�F���Z�b�g
	/// </summary>
	/// <param name="lightPos">���C�g�F</param>
	inline void SetLightColor(const XMFLOAT3& lightColor)
	{
		m_lightColor = lightColor;
	}

	/// <summary>
	/// ���C�g���������W�����擾
	/// </summary>
	/// <returns>���C�g���������W��</returns>
	inline const XMFLOAT3& GetLightAtten() { return m_lightAtten; }

	/// <summary>
	/// ���C�g���������W�����Z�b�g
	/// </summary>
	/// <param name="lightPos">���C�g���������W��</param>
	inline void SetLightAtten(const XMFLOAT3& lightAtten)
	{
		m_lightAtten = lightAtten;
	}

	/// <summary>
	/// ���C�g�p�x�������擾
	/// </summary>
	/// <returns>���C�g�p�x����</returns>
	inline const XMFLOAT2& GetLightFactorAngleCos() { return m_lightFactorAngleCos; }

	/// <summary>
	/// ���C�g�p�x�������Z�b�g
	/// </summary>
	/// <param name="lightFactorAngleCos">���C�g�����p�x</param>
	inline void SetLightFactorAngleCos(const XMFLOAT2& lightFactorAngleCos)
	{
		m_lightFactorAngleCos.x = cosf(DirectX::XMConvertToRadians(lightFactorAngleCos.x));
		m_lightFactorAngleCos.y = cosf(DirectX::XMConvertToRadians(lightFactorAngleCos.y));
	}

	/// <summary>
	/// �L���t���O���擾
	/// </summary>
	/// <returns>�L���t���O</returns>
	inline bool GetActive() { return m_active; };

	/// <summary>
	/// �L���t���O���Z�b�g
	/// </summary>
	/// <param name="active">�L���t���O</param>
	inline void SetActive(const bool active)
	{
		m_active = active;
	}
};