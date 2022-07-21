#pragma once
#include "FbxModel.h"
#include "FbxLoader.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FbxObject3d
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using string = std::string;

public: //�萔
	//�{�[���̍ő吔
	static const int MAX_BONES = 32;

public: //�T�u�N���X
	//�萔�o�b�t�@�\����
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj; //�r���[�v���W�F�N�V�����s��
		XMMATRIX world; //���[���h�s��
		XMFLOAT3 cameraPos; //�J�������W�i���[���h���W�j
	};

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* dev;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�J����
	static Camera* camera;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	//FBX�ǂݍ���
	static FbxLoader* fbxLoader;

public: //�ÓI�����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FbxObject3d();

	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static FbxObject3d* GetInstance();

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <returns>����</returns>
	static bool StaticInitialize(ID3D12Device* dev);

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// �`��O����
	/// </summary>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// FBX�쐬
	/// </summary>
	/// <param name="modelName">FBX���f����</param>
	/// <returns></returns>
	static FbxObject3d* CreateFBXObject(const string& modelName);

private: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBufferTransform;
	//�萔�o�b�t�@�i�X�L���j
	ComPtr<ID3D12Resource> constBufferSkin;
	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1, 1, 1 };
	//���[�J����]�p
	XMFLOAT3 rotation = { 0, 0, 0 };
	//���[�J�����W
	XMFLOAT3 position = { 0, 0, 0 };
	//���[�J�����[���h�s��
	XMMATRIX matWorld;
	//FBX���f��
	FbxModel* fbxModel = nullptr;
	//1�t���[��
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ����i�A�j���[�V�����j
	FbxTime currentTime;
	//�A�j���[�V�����t���O
	bool isPlay = false;
	//�A�j���[�V�������[�v�t���O
	bool isLoop = false;

public: //�����o�֐�
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
	void Draw();

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	/// <param name="loop">���[�v�̐���</param>
	void PlayAnimation(bool loop = false);

	/// <summary>
	/// �A�j���[�V���������Z�b�g
	/// </summary>
	void ResetAnimation();

public: //�A�N�Z�b�T
	/// <summary>
	/// ���W
	/// </summary>
	XMFLOAT3 GetPosition() { return position; }
	void SetPosition(XMFLOAT3 position);

	/// <summary>
	/// ��]
	/// </summary>
	XMFLOAT3 GetRotation() { return rotation; }
	void SetRotation(XMFLOAT3 rotation);

	/// <summary>
	/// �X�P�[��
	/// </summary>
	XMFLOAT3 GetScale() { return scale; }
	void SetScale(XMFLOAT3 scale);

	/// <summary>
	/// �A�j���[�V������
	/// </summary>
	bool GetIsPlay() { return isPlay; }

	/// <summary>
	/// ���[�v
	/// </summary>
	bool GetLoop() { return isLoop; }
	void SetLoop(bool loop);

	/// <summary>
	/// FBX���f���̃Z�b�g
	/// </summary>
	/// <param name="model">FBX���f��</param>
	void SetFBXModel(FbxModel* fbxModel) { this->fbxModel = fbxModel; }
};