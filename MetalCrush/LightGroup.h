#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"

#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

class LightGroup
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �萔
	static const int c_pointLightNum = 3;
	static const int c_dirLightNum = 3;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		// �����̐F
		XMFLOAT3 ambientColor;
		float pad1;
		// ���s�����p
		//DirectionalLight::ConstBufferData dirLights[c_dirLightNum];
		// �_�����p
		PointLight::ConstBufferData pointLights[c_pointLightNum];
	};

private: // �ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* dev;

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
	ComPtr<ID3D12Resource> constBuff;
	// ���C�g�̃x�N�g��
	XMVECTOR lightDir = { 1, 0, 0, 0 };
	// �F
	XMFLOAT3 lightColor = { 1, 1, 1 };
	// �_�[�e�B�[�t���O
	bool dirty = false;
	// �_�[�e�B�[�t���O����������
	bool isDirty = true;

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
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	/// <summary>
	/// ���C�g�X�V�̐���
	/// </summary>
	/// <returns>����</returns>
	bool GetDirty() { return isDirty; };
};