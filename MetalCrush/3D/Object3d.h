#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "CollisionInfo.h"
#include "PipelineManager.h"

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
	static ID3D12Device* s_dev;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* s_cmdList;
	//�J�����N���X
	static Camera *s_camera;
	//���C�g�N���X
	static Light* s_light;

public: //�ÓI�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	static Object3d* Create(const std::string& modelName = "defaultTexture", bool smooting = false);

	/// <summary>
	/// �J�����Z�b�g
	/// </summary>
	static Camera* GetCamera() { return s_camera; };

	/// <summary>
	/// ���C�g�Z�b�g
	/// </summary>
	static void SetLight(Light* light) { Object3d::s_light = light; }

	/// <summary>
	/// �`��O����
	/// </summary>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

private: //�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMMATRIX viewproj; //�r���[�v���W�F�N�V�����s��
		XMMATRIX world; //���[���h�s��
		XMFLOAT3 cameraPos; //�J�������W(���[���h���W)
		float pad; //�p�f�B���O
		XMFLOAT4 color; //�F(RGBA)
	};

private: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> m_constBuff;
	//���s�ړ�
	XMFLOAT3 m_position = { 0.0f, 0.0f, 0.0f };
	//�X�P�[��
	XMFLOAT3 m_scale = { 1.0f, 1.0f, 1.0f };
	//��]
	XMFLOAT3 m_rotation = { 0.0f, 0.0f, 0.0f };
	//�F(RGBA)
	XMFLOAT4 m_color = { 1, 1, 1, 1 };
	//���[���h�s��
	XMMATRIX m_matWorld;
	//���f���f�[�^
	Model* m_model = nullptr;
	// �r���{�[�h
	bool m_isBillboard = false;
	//�N���X���i�f�o�b�O�p�j
	const char* m_name = nullptr;
	//�R���C�_�[
	std::unique_ptr<BaseCollider> m_collider = nullptr;
	//�_�[�e�B�[�t���O
	bool m_dirty = true;

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
	XMFLOAT3 GetPosition() { return m_position; }
	void SetPosition(XMFLOAT3 position);

	//void SetOffset(XMFLOAT2 offset);

	/// <summary>
	/// ��]
	/// </summary>
	XMFLOAT3 GetRotation() { return m_rotation; }
	void SetRotation(XMFLOAT3 rotation);

	/// <summary>
	/// �X�P�[��
	/// </summary>
	XMFLOAT3 GetScale() { return m_scale; }
	void SetScale(XMFLOAT3 scale);

	/// <summary>
	/// �F
	/// </summary>
	XMFLOAT4 GetColor() { return m_color; }
	void SetColor(XMFLOAT4 color);

	/// <summary>
	/// ���f���f�[�^�Z�b�g
	/// </summary>
	void SetModel(Model* model) { this->m_model = model; }

	/// <summary>
	/// �r���{�[�h�Z�b�g
	/// </summary>
	void SetBillboard(bool isBillboard) { this->m_isBillboard = isBillboard; }

	/// <summary>
	/// ���[���h�s����擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const XMMATRIX& GetMatWorld() { return m_matWorld; }

	/// <summary>
	/// �R���C�_�[�̃Z�b�g
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// �V�F�[�_�[�Z�b�g
	/// </summary>
	/// <param name="shaderType">�V�F�[�_�[�̎��</param>
	void SetShader(const int shaderType);

	/// <summary>
	/// �T�u�e�N�X�`���̃Z�b�g
	/// </summary>
	/// <param name="filename">�t�@�C����</param>
	void SetSubTexture(const std::string& filename);

	/// <summary>
	/// �}�X�N�e�N�X�`���̃Z�b�g
	/// </summary>
	/// <param name="filename">�t�@�C����</param>
	void SetMaskTexture(const std::string& filename);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo& info) {};
};