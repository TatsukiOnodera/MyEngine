#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "CollisionInfo.h"

class BaseCollider;

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
	//���C�g�N���X
	static Light* light;

public: //�ÓI�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

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
	static Object3d* Create(const std::string& modelName = "defaultTexture", bool smooting = false);

	/// <summary>
	/// �J�����Z�b�g
	/// </summary>
	static Camera* GetCamera() { return camera; };

	/// <summary>
	/// ���C�g�Z�b�g
	/// </summary>
	static void SetLight(Light* light) { Object3d::light = light; }

public: //�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMMATRIX viewproj; //�r���[�v���W�F�N�V�����s��
		XMMATRIX world; //���[���h�s��
		XMFLOAT3 cameraPos; //�J�������W(���[���h���W)
		XMFLOAT4 color; //�F(RGBA)
	};

protected: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���s�ړ�
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	//�X�P�[��
	XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
	//��]
	XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
	//�F(RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//���[���h�s��
	XMMATRIX matWorld;
	//���f���f�[�^
	Model* model = nullptr;
	// �r���{�[�h
	bool isBillboard = false;
	//�N���X���i�f�o�b�O�p�j
	const char* name = nullptr;
	//�R���C�_�[
	BaseCollider* collider = nullptr;

public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Object3d() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Object3d();

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

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

	/// <summary>
	/// ���f���f�[�^�Z�b�g
	/// </summary>
	void SetModel(Model* model) { this->model = model; }

	/// <summary>
	/// �r���{�[�h�Z�b�g
	/// </summary>
	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

	/// <summary>
	/// ���[���h�s����擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }

	/// <summary>
	/// �R���C�_�[�̃Z�b�g
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo& info) {};
};