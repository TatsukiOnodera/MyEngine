#pragma once
#include <fbxsdk.h>
#include <cassert>
#include <string.h>
#include <d3d12.h>
#include <d3dx12.h>

class FbxLoader
{
private: //�G�C���A�X
	//std::���ȗ�
	using string = std::string;

public: //�萔
	//���f���i�[���[�g�p�X
	static const string baseDirectory;

private: //�����o�ϐ�
//�f�o�C�X
	ID3D12Device* dev = nullptr;
	//FBX�}�l�[�W��
	FbxManager* fbxManager = nullptr;
	//FBX�C���|�[�^
	FbxImporter* fbxImporter = nullptr;

public: //�ÓI�����o�֐�
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static FbxLoader* GetInstance();

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device">D3D12�f�o�C�X</param>
	bool Initialize(ID3D12Device* device);

	/// <summary>
	/// ��n��
	/// </summary>
	void Finalize();

	/// <summary>
	/// �t�@�C������FBX���f����ǂݍ���
	/// </summary>
	/// <param name="modelName">���f����</param>
	void LoadModelFromFile(const string& modelName);
};
