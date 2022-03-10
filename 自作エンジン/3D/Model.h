#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

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
	static ID3D12Device* device;

	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;

private: //�����o�ϐ�
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;

	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};

	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;

	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

	//�}�e���A��
	Material material;

	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;

	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;

	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;

public: //�ÓI�����o�֐�
	//�ÓI������
	static void StaticInitialize(ID3D12Device* device);

	//���f������
	static Model* CreateModel(const std::string& modelName = "defaultTexture");

public: //�����o�֐�
	//�}�e���A���̎擾
	Material GetMaterial() { return material; }

	//�f�X�N���v�^�q�[�v�̏�����
	void InitializeDescriptorHeap();

	//�e�N�X�`���ǂݍ���
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	//���f���쐬
	void InitializeModel(const std::string& modelName);

	//�}�e���A���̓ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	//�������E�X�V�E�`��
	void Initialize();
	void Update(Material material);
	void Draw(ID3D12GraphicsCommandList* cmdList);
};