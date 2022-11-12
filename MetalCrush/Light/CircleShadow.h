#pragma once
#include <DirectXMath.h>

struct CircleShadow
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
		XMVECTOR dir;
		XMFLOAT3 casterPos;
		float distanceCasterLight;
		XMFLOAT3 atten;
		float pad1;
		XMFLOAT2  factorAngleCos;
		unsigned int active;
		float pad2;
	};

private: // �����o�ϐ�
	// �x�N�g��
	XMVECTOR m_dir = { 1, 0, 0, 0 };
	// �L���X�^�[�ƃ��C�g�̋���
	float m_distanceCasterLight = 100.0f;
	// �L���X�^�[���W
	XMFLOAT3 m_casterPos = { 0, 0, 0 };
	// ���������W��
	XMFLOAT3 m_atten = { 0.5f, 0.6f, 0.0f };
	// �����p�x�i�J�n�p�x�A�I���p�x�j
	XMFLOAT2 m_factorAngleCos = { 0.2f, 0.5f };
	// �L���t���O
	bool m_active = false;

public: // �A�N�Z�b�T
	/// <summary>
	/// �x�N�g�����擾
	/// </summary>
	/// <returns>�x�N�g��</returns>
	inline XMVECTOR GetDir() { return m_dir; }

	/// <summary>
	/// �x�N�g�����Z�b�g
	/// </summary>
	/// <param name="dir">�x�N�g��</param>
	inline void SetDir(const XMVECTOR& dir)
	{
		m_dir = DirectX::XMVector3Normalize(dir);
	}

	/// <summary>
	/// �L���X�^�[���W���擾
	/// </summary>
	/// <returns>�L���X�^�[���W</returns>
	inline XMFLOAT3 GetCasterPos() { return m_casterPos; }

	/// <summary>
	/// �L���X�^�[���W���Z�b�g
	/// </summary>
	/// <param name="casterPos">�L���X�^�[���W</param>
	inline void SetCasterPos(const XMFLOAT3& casterPos)
	{
		m_casterPos = casterPos;
	}

	/// <summary>
	/// �L���X�^�[�ƃ��C�g�̋������擾
	/// </summary>
	/// <returns>�L���X�^�[�ƃ��C�g�̋���</returns>
	inline float GetDistanceCasterLight() { return m_distanceCasterLight; }

	/// <summary>
	/// �L���X�^�[�ƃ��C�g�̋������Z�b�g
	/// </summary>
	/// <param name="distanceCasterLight">�L���X�^�[�ƃ��C�g�̋���</param>
	inline void SetDistanceCasterLight(float distanceCasterLight)
	{
		m_distanceCasterLight = distanceCasterLight;
	}

	/// <summary>
	/// ���������W�����擾
	/// </summary>
	/// <returns>���������W��</returns>
	inline const XMFLOAT3& GetAtten() { return m_atten; }

	/// <summary>
	/// ���������W�����Z�b�g
	/// </summary>
	/// <param name="atten">���������W��</param>
	inline void SetAtten(const XMFLOAT3& atten)
	{
		m_atten = atten;
	}

	/// <summary>
	/// ���������W�����擾
	/// </summary>
	/// <returns>���������W��</returns>
	inline const XMFLOAT2& GetFactorAngleCos() { return m_factorAngleCos; }

	/// <summary>
	/// ���C�g���������W�����Z�b�g
	/// </summary>
	/// <param name="FactorAngleCos">�����p�x</param>
	inline void SetFactorAngleCos(const XMFLOAT2& factorAngleCos)
	{
		m_factorAngleCos.x = cosf(DirectX::XMConvertToRadians(factorAngleCos.x));
		m_factorAngleCos.y = cosf(DirectX::XMConvertToRadians(factorAngleCos.y));
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