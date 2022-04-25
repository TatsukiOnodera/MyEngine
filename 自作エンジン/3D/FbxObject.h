#pragma once
#include "FbxModel.h"
#include "Camera.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FbxObject
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

public: //�T�u�N���X
	//�萔�o�b�t�@�\����
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj; //�r���[�v���W�F�N�V�����s��
		XMMATRIX world; //���[���h�s��
		XMFLOAT3 cameraPos; //�J�������W�i���[���h���W�j
	};

private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* dev;
	//�J����
	static Camera* camera;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;

public: //�ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static FbxObject *GetInstance();

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
	/// FBX�쐬
	/// </summary>
	/// <param name="modelName">FBX���f����</param>
	/// <returns></returns>
	static FbxModel* CreateFBXModel(const string& modelName);

protected: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBufferTransform;
	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1, 1, 1 };
	//���[�J����]�p
	XMFLOAT3 rotation = { 0, 0, 0 };
	//���[�J�����W
	XMFLOAT3 position = { 0, 0, 0 };
	//���[�J�����[���h�s��
	XMMATRIX matWorld;
	//FBX���f��
	FbxModel* model = nullptr;
	//FBX���[�_�[
	FbxLoader* loader = nullptr;

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
	void Draw(ID3D12GraphicsCommandList* cmdList);

public: //�A�N�Z�b�T
	/// <summary>
	/// FBX���f���̃Z�b�g
	/// </summary>
	/// <param name="model">FBX���f��</param>
	void SetFBXModel(FbxModel* model) { this->model = model; }

	void SetDevice(ID3D12Device* dev) { this->dev = dev; }
	void SetCamera(Camera* camera) { this->camera = camera; }
};