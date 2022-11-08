#pragma once
#include <DirectXMath.h>

class DirectionalLight
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
		XMFLOAT3 lightColor;
		unsigned int active;
	};

private: // �����o�ϐ�
	// ���C�g�̃x�N�g��
	XMVECTOR m_lightDir = { 1, 0, 0, 0 };
	// ���C�g�̐F
	XMFLOAT3 m_lightColor = { 1, 1, 1 };
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
		m_lightDir = lightDir;
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
