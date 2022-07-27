#pragma once
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>
#include "FbxModelMesh.h"

//�m�[�h�\����
struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1, 1, 1, 0 };
	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0, 0, 0, 0 };
	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0, 0, 0, 1 };
	//���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;
	//�e�m�[�h
	Node* parent = nullptr;
};

class FbxModel
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::���ȗ�
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public: //������
	//�t�����h�N���X�����
	friend class FbxLoader;

public: //�T�u�N���X
	//�{�[���\����
	struct Bone
	{
		//���O
		std::string name;
		//�����p���̋t�s��
		DirectX::XMMATRIX invInitialPose;
		//�N���X�^�[�iFBX���̃{�[�����j
		FbxCluster* fbxCluster;
		//�R���X�g���N�^
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

private: //���f���f�[�^�p�ϐ�
	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<Node>nodes;
	//���b�V�������m�[�h
	Node* meshNode = nullptr;
	//�A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = { 1, 1, 1 };
	//�f�B�t���[�Y�W��
	DirectX::XMFLOAT3 diffuse = { 1, 1, 1 };
	//�e�N�X�`�����^�f�[�^
	DirectX::TexMetadata metadata = {};
	//�X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg = {};
	//�{�[���z��
	std::vector<Bone> bones;
	//FBX�V�[��
	FbxScene* fbxScene = nullptr;
	//���b�V���z��
	std::vector<FbxModelMesh*> meshes;

private: //�����o�ϐ�
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

public: //�����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FbxModel();

	/// <summary>
	/// �e�N�X�`���o�b�t�@����
	/// </summary>
	void CreateBuffers(ID3D12Device* dev);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// ���f���̕ό`�s��擾
	/// </summary>
	/// <returns>���f���̕ό`�s��</returns>
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

	/// <summary>
	/// ���f���̕ό`�t�s��擾
	/// </summary>
	/// <returns>���f���̋t�ό`�s��</returns>
	const XMMATRIX& GetInverseGlobalTransform();

	/// <summary>
	/// �{�[���z��̎擾
	/// </summary>
	/// <returns>�{�[���z��</returns>
	std::vector<Bone>& GetBones() { return bones; }

	/// <summary>
	/// FBX�V�[���̎擾
	/// </summary>
	/// <returns>FBX�V�[��</returns>
	FbxScene* GetFbxScene() { return fbxScene; }

	/// <summary>
	/// ���b�V���ǉ�
	/// </summary>
	void AddModelMesh(FbxModelMesh* modelMesh);
};