#pragma once
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

class FbxModelMesh
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //�萔
	//�{�[���C���f�b�N�X�̍ő�l
	static const int MAX_BONE_INDICES = 4;

public: //�T�u�N���X
	//���_�f�[�^�\����
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos; //XYZ���W
		DirectX::XMFLOAT3 normal; //�@���x�N�g��
		DirectX::XMFLOAT2 uv; //UV���W
		UINT boneIndex[MAX_BONE_INDICES]; //�{�[���ԍ�
		float boneWeight[MAX_BONE_INDICES]; //�{�[���d��
	};

public: //�����o�ϐ�
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//���_�f�[�^�z��
	std::vector<VertexPosNormalUvSkin> vertices;
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;

public: //�����o�֐�
	/// <summary>
	/// �o�b�t�@����
	/// </summary>
	void CreateBuffers(ID3D12Device* dev);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);
};