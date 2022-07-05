#pragma once
#include "FbxModel.h"
#include "FbxModelMesh.h"
#include <fbxsdk.h>
#include <vector>
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
	/// <summary>
	/// ���_���W�ǂݎ��
	/// </summary>
	/// <param name="fbxModel">�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="fbxMesh">��͑Ώۂ̃��b�V��</param>
	/// <param name="modelMesh">���b�V���f�[�^</param>
	void ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh, FbxModelMesh* modelMesh);

	/// <summary>
	/// �ʏ��ǂݎ��
	/// </summary>
	/// <param name="fbxModel">�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="fbxMesh">��͑Ώۂ̃��b�V��</param>
	/// <param name="modelMesh">���b�V���f�[�^</param>
	void ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh, FbxModelMesh* modelMesh);

	/// <summary>
	/// �}�e���A���ǂݎ��
	/// </summary>
	/// <param name="fbxModel">�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="fbxNode">��͑Ώۂ̃m�[�h</param>
	void ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode);

	/// <summary>
	/// �e�N�X�`���ǂݎ��
	/// </summary>
	/// <param name="fbxModel">�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="fullpath">�A�h���X</param>
	void LoadTexture(FbxModel* fbxModel, const std::string& fullpath);

	/// <summary>
	/// �f�B���N�g�����܂񂾃t�@�C���p�X����t�@�C�����𒊏o
	/// </summary>
	/// <param name="path">�t�@�C���p�X</param>
	/// <returns>�t�@�C����</returns>
	std::string ExtractFileName(const std::string& path);

	/// <summary>
	/// �X�L�j���O���̓ǂݎ��
	/// </summary>
	/// <param name="fbxModel">�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="fbxMesh">��͑Ώۂ̃��b�V��</param>
	/// <param name="modelMesh">���b�V���f�[�^</param>
	void ParseSkin(FbxModel* fbxModel, FbxMesh* fbxMesh, FbxModelMesh* modelMesh);

public: //�ÓI�����o�֐�
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// FBX�s���XMMatrix�ɕϊ�
	/// </summary>
	/// <param name="dst">�������ݐ�</param>
	/// <param name="src">���ɂȂ�FBX�s��</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

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
	FbxModel* LoadModelFromFile(const string& modelName);

	/// <summary>
	/// �ċA�I�Ƀm�[�h�\�������
	/// </summary>
	/// <param name="fbxModel">�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="fbxNode">��͑Ώۂ̃m�[�h</param>
	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// ���b�V���ǂݎ��
	/// </summary>
	/// <param name="fbxModel">�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="fbxNode">��͑Ώۂ̃m�[�h</param>
	void ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode);
};