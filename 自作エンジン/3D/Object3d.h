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
	//�ÓI������
	static bool StaticInitialize(ID3D12Device* device, Camera* camera, int window_width, int window_height);

	//�O���t�B�b�N�p�C�v���C���̐���
	static void CreateGraphicsPipeline();

	//�`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	//�`��㏈��
	static void PostDraw();

	//�I�u�W�F�N�g����
	static Object3d* Create(const std::string& modelName);

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

public: //�A�N�Z�b�T
	//���W
	XMFLOAT3 GetPosition() { return position; }
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	//��]
	XMFLOAT3 GetRotation() { return rotation; }
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	//�X�P�[��
	XMFLOAT3 GetScale() { return scale; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	//�F
	XMFLOAT4 GetColor() { return color; }
	void SetColor(XMFLOAT4 color) { this->color = color; }

	//���f���f�[�^�Z�b�g
	void SetModel(Model* model) { this->model = model; }

public: //�����o�֐�
	//�������E�X�V�E�`��
	void Initialize();
	void Update();
	void Draw();
};