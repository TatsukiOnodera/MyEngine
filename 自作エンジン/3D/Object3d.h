#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "Model.h"
#include "Camera.h"

class Object3d
{
public: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //�ÓI�ϐ�;
		 // �f�o�C�X
	static ID3D12Device* dev;

	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;

	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;  

	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;

	//�J�����N���X
	static Camera *camera; 

public: //�ÓI�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	static bool StaticInitialize(ID3D12Device* device, Camera* camera, int window_width, int window_height);

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
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
	/// �I�u�W�F�N�g����
	/// </summary>
	static Object3d* Create(const std::string& modelName);

	/// <summary>
	/// �I�u�W�F�N�g
	/// </summary>
	/// <returns></returns>
	static Camera* GetCamera() { return camera; };

public: //�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMMATRIX mat; //�s��
		XMFLOAT4 color; //�F(RGBA)
	};

private: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;

	//���s�ړ�
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f }; //���W��ϐ��Ƃ��Ď���

	//�X�P�[��
	XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f }; //���W��ϐ��Ƃ��Ď���

	//��]
	XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f }; //���W��ϐ��Ƃ��Ď���

	//�F(RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };

	//���[���h�s��
	XMMATRIX matWorld;

	//���f���f�[�^
	Model* model = nullptr;

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
	/// �F
	/// </summary>
	XMFLOAT4 GetColor() { return color; }
	void SetColor(XMFLOAT4 color);

	//���f���f�[�^�Z�b�g
	void SetModel(Model* model) { this->model = model; }
};