#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>

class Material
{
private: // �G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT3 ambient; // �A���r�G���g�W��
		float pad1; // �p�f�B���O
		XMFLOAT3 diffuse; // �f�B�t���[�Y�W��
		float pad2; // �p�f�B���O
		XMFLOAT3 specular; // �X�y�L�����[�W��
		float alpha; // �A���t�@
	};

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* s_dev;

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* dev);

	/// <summary>
	/// �}�e���A������
	/// </summary>
	/// <returns>�}�e���A��</returns>
	static Material* Create();

public: // �}�e���A���f�[�^
	// �}�e���A����
	std::string name;
	// �A���r�G���g�e���x
	XMFLOAT3 ambient;	
	// �f�B�t���[�Y�e���x
	XMFLOAT3 diffuse;
	// �X�y�L�����[�e���x
	XMFLOAT3 specular;
	// �A���t�@
	float alpha;
	// �e�N�X�`���t�@�C����
	std::string textureFilename;

private: // �����o�ϐ�
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> m_texbuff;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> m_constBuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpuDescHandleSRV;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Material();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �萔�o�b�t�@�̐���
	/// </summary>
	void CreateConstBuffer();

	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="directoryPath">�f�B���N�g���p�X</param>
	/// <param name="cpuDescHandleSRV">�V�F�[�_���\�[�X�r���[�̃n���h��(CPU)</param>
	/// <param name="gpuDescHandleSRV">�V�F�[�_���\�[�X�r���[�̃n���h��(CPU)</param>
	void LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV);

public: // �A�N�Z�b�T
	/// <summary>
	/// �萔�o�b�t�@�̎擾
	/// </summary>
	/// <returns>�萔�o�b�t�@</returns>
	inline ID3D12Resource* GetConstantBuffer() { return m_constBuff.Get(); }

	/// <summary>
	/// �V�F�[�_���\�[�X�r���[�̃n���h���iCPU�j�擾
	/// </summary>
	/// <returns>�V�F�[�_���\�[�X�r���[�̃n���h���iCPU�j</returns>
	inline const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCPUHandle() { return m_cpuDescHandleSRV; }

	/// <summary>
	/// �V�F�[�_���\�[�X�r���[�̃n���h���iGPU�j�擾
	/// </summary>
	/// <returns>�V�F�[�_���\�[�X�r���[�̃n���h���iGPU�j</returns>
	inline const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGPUHandle() { return m_gpuDescHandleSRV; }
};