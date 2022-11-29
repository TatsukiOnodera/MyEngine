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
private: // �G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �萔
	// ���s�����̐�
	static const int c_pointLightNum = 3;
	// �_�����̐�
	static const int c_dirLightNum = 3;
	// �X�|�b�g���C�g�̐�
	static const int c_spotLightNum = 3;
	// �ۉe�̐�
	static const int c_circleShadowNum = 13;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		// �����̐F
		XMFLOAT3 ambientColor;
		float pad1;
		// ���s�����p
		DirectionalLight::ConstBufferData dirLights[c_dirLightNum];
		// �_�����p
		PointLight::ConstBufferData pointLights[c_pointLightNum];
		// �X�|�b�g���C�g�p
		SpotLight::ConstBufferData spotLights[c_spotLightNum];
		// �ۉe�p
		CircleShadow::ConstBufferData circleShadows[c_circleShadowNum];
	};

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* s_dev;

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	static bool StaticInitialize(ID3D12Device* dev);

	/// <summary>
	/// ����
	/// </summary>
	static LightGroup* Create();

private: // �����o�ϐ�
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> m_constBuff;
	// �����̐F
	XMFLOAT3 m_ambientColor = { 1,1,1 };
	// ���s�����̔z��
	DirectionalLight m_dirLights[c_dirLightNum];
	// �_�����̔z��
	PointLight m_pointLights[c_pointLightNum];
	// �X�|�b�g���C�g�̔z��
	SpotLight m_spotLights[c_spotLightNum];
	// �ۉe�̍s��
	CircleShadow m_circleShadows[c_circleShadowNum];
	// �_�[�e�B�[�t���O
	bool m_dirty = false;
	// �_�[�e�B�[�t���O����������
	bool m_isDirty = false;

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �萔�o�b�t�ɓ]��
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// �W���̃��C�g�ݒ�
	/// </summary>
	void DefaultLightSetting();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

public: // �A�N�Z�b�T
	/// <summary>
	/// �����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="color">���C�g�F</param>
	void SetAmbientColor(const XMFLOAT3& color);

	/// <summary>
	/// ���s�����̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetDirLightActive(int index, bool active);

	/// <summary>
	/// ���s�����̃��C�g�������Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightDir">���C�g����</param>
	void SetDirLightDir(int index, const XMVECTOR& lightDir);

	/// <summary>
	/// ���s�����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightColor">���C�g�F</param>
	void SetDirLightColor(int index, const XMFLOAT3& lightColor);

	/// <summary>
	/// �_�����̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetPointLightActive(int index, bool active);

	/// <summary>
	/// �_�����̃��C�g���W���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightPos">���C�g���W</param>
	void SetPointLightPos(int index, const XMFLOAT3& lightPos);

	/// <summary>
	/// �_�����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightColor">���C�g�F</param>
	void SetPointLightColor(int index, const XMFLOAT3& lightColor);

	/// <summary>
	/// �_�����̃��C�g���������W�����Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightAtten">���C�g���������W��</param>
	void SetPointLightAtten(int index, const XMFLOAT3& lightAtten);

	/// <summary>
	/// �X�|�b�g���C�g�̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetSpotLightActive(int index, bool active);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g�������Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightDir">���C�g����</param>
	void SetSpotLightDir(int index, const XMVECTOR& lightDir);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g���W���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightPos">���C�g���W</param>
	void SetSpotLightPos(int index, const XMFLOAT3& lightPos);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightColor">���C�g�F</param>
	void SetSpotLightColor(int index, const XMFLOAT3& lightColor);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g���������W�����Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightAtten">���C�g���������W��</param>
	void SetSpotLightAtten(int index, const XMFLOAT3& lightAtten);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g�����p�x���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightAtten">���C�g�����p�x</param>
	void SetSpotLightFactorAngleCos(int index, const XMFLOAT2& lightFactorAngleCos);

	/// <summary>
	/// �ۉe�̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">�ۉe�̔ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetCircleShadowActive(int index, bool active);

	/// <summary>
	/// �ۉe�̃L���X�^�[���W���Z�b�g
	/// </summary>
	/// <param name="index">�ۉe�̔ԍ�</param>
	/// <param name="casterPos">�L���X�^�[���W</param>
	void SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos);

	/// <summary>
	/// �ۉe�̕������Z�b�g
	/// </summary>
	/// <param name="index">�ۉe�̔ԍ�</param>
	/// <param name="lightDir">����</param>
	void SetCircleShadowDir(int index, const XMVECTOR& lightDir);

	/// <summary>
	/// �ۉe�̃L���X�^�[�ƃ��C�g�̋���
	/// </summary>
	/// <param name="index">�ۉe�̔ԍ�</param>
	/// <param name="distanceCasterLight">�L���X�^�[�ƃ��C�g�̋���</param>
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);

	/// <summary>
	/// �ۉe�̋��������W�����Z�b�g
	/// </summary>
	/// <param name="index">�ۉe�̔ԍ�</param>
	/// <param name="lightAtten">���������W��</param>
	void SetCircleShadowAtten(int index, const XMFLOAT3& lightAtten);

	/// <summary>
	/// �ۉe�̌����p�x���Z�b�g
	/// </summary>
	/// <param name="index">�ۉe�̔ԍ�</param>
	/// <param name="lightAtten">�����p�x</param>
	void SetCircleShadowFactorAngleCos(int index, const XMFLOAT2& lightFactorAngleCos);

	/// <summary>
	/// ���C�g�X�V�̐���
	/// </summary>
	/// <returns>����</returns>
	inline bool GetDirty() { return m_isDirty; };
};