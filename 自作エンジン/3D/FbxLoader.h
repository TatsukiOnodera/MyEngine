#pragma once
#include "FbxModel.h"

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
	//�e�N�X�`�����Ȃ��ꍇ�̕W���e�N�X�`���t�@�C����
	static const string defaultTextureFileName;

private: //�����o�ϐ�
//�f�o�C�X
	ID3D12Device* dev = nullptr;
	//FBX�}�l�[�W��
	FbxManager* fbxManager = nullptr;
	//FBX�C���|�[�^
	FbxImporter* fbxImporter = nullptr;

private: //�T�u�֐�
	//���_���W�ǂݎ��
	void ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//�ʏ��ǂݎ��
	void ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//�}�e���A���ǂݎ��
	void ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode);
	//�e�N�X�`���ǂݎ��
	void LoadTexture(FbxModel* fbxModel, const std::string& fullpath);
	//�f�B���N�g�����܂񂾃t�@�C���p�X����t�@�C�����𒊏o
	std::string ExtractFileName(const std::string& path);

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

	/// <summary>
	/// �ċA�I�Ƀm�[�h�\�������
	/// </summary>
	/// <param name="fbxModel"></param>
	/// <param name="fbxNode"></param>
	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// ���b�V���ǂݎ��
	/// </summary>
	/// <param name="model">�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="fbxNode">��͑Ώۂ̃m�[�h</param>
	void ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode);
};