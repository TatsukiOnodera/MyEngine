#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <unordered_map>

#include "PipelineManager.h"

class Model
{
private: //�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //�萔
	static const int maxTexNum = 3;

public: //�T�u�N���X
	//���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT3 ambient; //�A���r�G���g�W��
		float pad1; //�p�f�B���O
		XMFLOAT3 diffuse; //�f�B�t���[�Y�W��
		float pad2; //�p�f�B���O
		XMFLOAT3 specular; //�X�y�L�����[�W��
		float alpha; //�A���t�@
	};
	//�}�e���A��
	struct Material
	{
		std::string name; //�}�e���A����
		XMFLOAT3 ambient; //�A���r�G���g�e���x
		XMFLOAT3 diffuse; //�f�B�t���[�Y�e���x
		XMFLOAT3 specular; //�X�y�L�����[�e���x
		float alpha; //�A���t�@
		std::string textureFilename = "defaultTexture"; //�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.5, 0.5, 0.5 };
			diffuse = { 0.0, 0.0, 0.0 };
			specular = { 0.0, 0.0, 0.0 };
			alpha = 1.0;
		}
	};

private: //�ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* s_dev;
	// �f�X�N���v�^�T�C�Y
	static UINT s_descriptorHandleIncrementSize;

public: //�ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize(ID3D12Device* dev);

	/// <summary>
	/// ���f������
	/// </summary>
	static Model* CreateModel(const std::string& modelName, bool smooting);

private: //�����o�ϐ�
	//�O���t�B�b�N�X�p�C�v���C��
	std::unique_ptr<PipelineManager> m_graphicsPipeline = nullptr;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> m_vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> m_indexBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW m_vbView = {};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW m_ibView = {};
	// ���_�f�[�^�z��s
	std::vector<VertexPosNormalUv> m_vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> m_indices;
	//�}�e���A��
	Material m_material;
	//�e�N�X�`����
	std::vector<std::string> textureName;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> m_texbuff[maxTexNum];
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> m_constBuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuDescHandleSRV[maxTexNum];
	// �V�F�[�_���\�[�X�r���[�̃n���h��(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpuDescHandleSRV[maxTexNum];
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> m_descHeap;
	//���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> m_smoothData;

public: //�����o�֐�
	/// <summary>
	/// �}�e���A���̎擾
	/// </summary>
	Material GetMaterial() { return m_material; }

	/// <summary>
	/// �e�N�X�`�����ǂݍ���
	/// </summary>
	void LoadTextureName(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture();

	/// <summary>
	/// ���f���쐬
	/// </summary>
	void InitializeModel(const std::string& modelName, bool smooting);

	/// <summary>
	/// �G�b�W�������f�[�^�̒ǉ�
	/// </summary>
	void AddSmoothData(unsigned short indexPosition, unsigned short indexVertex);

	/// <summary>
	/// ���������ꂽ���_�@���̌v�Z
	/// </summary>
	void CalculateSmoothedVertexNormals();

	/// <summary>
	/// �}�e���A���̓ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

public: //�A�N�Z�b�T
	/// <summary>
	/// �p�C�v���C���X�e�[�g���擾
	/// </summary>
	/// <returns>�p�C�v���C���X�e�[�g</returns>
	ID3D12PipelineState* GetPipelineState() { return m_graphicsPipeline->GetPipelineState(); }

	/// <summary>
	/// ���[�g�V�O�l�`�����擾
	/// </summary>
	/// <returns>���[�g�V�O�l�`��</returns>
	ID3D12RootSignature* GetRootSignature() { return m_graphicsPipeline->GetRootSignature(); }

	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C���Z�b�g
	/// </summary>
	/// <param name="shaderType">�V�F�[�_�[�̎��</param>
	void SetGraphicsPipeline(const int shaderType);

	/// <summary>
	/// �T�u�e�N�X�`���̃Z�b�g
	/// </summary>
	/// <param name="filename">�t�@�C����</param>
	void SetSubTexture(const std::string& filename);

	/// <summary>
	/// �}�X�N�e�N�X�`���̃Z�b�g
	/// </summary>
	/// <param name="filename">�t�@�C����</param>
	void SetMaskTexture(const std::string& filename);
};