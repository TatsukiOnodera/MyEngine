#pragma once
#include "Model.h"
#include "Camera.h"
#include "LightGroup.h"
#include "CollisionInfo.h"
#include "PipelineManager.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class BaseCollider;

class Object3d
{
public: // �G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //�ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* s_dev;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* s_cmdList;
	//�J�����N���X
	static Camera *s_camera;
	//���C�g�O���[�v�N���X
	static LightGroup* s_lightGroup;

public: //�T�u�N���X
//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMMATRIX viewproj; //�r���[�v���W�F�N�V�����s��
		XMMATRIX world; //���[���h�s��
		XMFLOAT3 cameraPos; //�J�������W(���[���h���W)
		float pad; //�p�f�B���O
		XMFLOAT4 color; //�F(RGBA)
		XMFLOAT2 tiling; // �^�C�����O
		XMFLOAT2 offset; // �I�t�Z�b�g
	};

	enum ShaderType
	{
		Phong, Toon, Mono, Blend, Specular, Bloom
	};

public: //�ÓI�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	static bool StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	static Object3d* Create(Model* model);

	/// <summary>
	/// �J�����Z�b�g
	/// </summary>
	inline static Camera* GetCamera() { return s_camera; };

	/// <summary>
	/// ���C�g�Z�b�g
	/// </summary>
	static void SetGroupLight(LightGroup* lightGroup) { Object3d::s_lightGroup = lightGroup; }

	/// <summary>
	/// �`��O����
	/// </summary>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

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
	// �^�C�����O
	XMFLOAT2 m_tiling = { 1, 1 };
	// �I�t�Z�b�g
	XMFLOAT2 m_offset = { 0, 0 };
	//���[���h�s��
	XMMATRIX m_matWorld = {};
	//���f���f�[�^
	Model* m_model = nullptr;
	// �r���{�[�h
	bool m_isBillboard = false;
	// �y�A�����g
	std::unique_ptr<Object3d> m_parent = nullptr;
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
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	inline XMFLOAT3& GetPosition() { return m_position; }

	/// <summary>
	/// ���W���Z�b�g
	/// </summary>
	/// <param name="position">���W</param>
	inline void SetPosition(const XMFLOAT3& position)
	{
		m_position = position;
		m_dirty = true;
	}

	/// <summary>
	/// ��]�p���擾
	/// </summary>
	/// <returns>��]</returns>
	inline XMFLOAT3& GetRotation() { return m_rotation; }

	/// <summary>
	/// ��]�p���Z�b�g
	/// </summary>
	/// <param name="rotation"></param>
	inline void SetRotation(const XMFLOAT3& rotation)
	{
		m_rotation = rotation;
		m_dirty = true;
	}

	/// <summary>
	/// �X�P�[�����擾
	/// </summary>
	/// <returns>�X�P�[��</returns>
	inline XMFLOAT3& GetScale() { return m_scale; }

	/// <summary>
	/// �X�P�[�����Z�b�g
	/// </summary>
	/// <param name="scale">�X�P�[��</param>
	inline void SetScale(const XMFLOAT3& scale)
	{
		m_scale = scale;
		m_dirty = true;
	}

	/// <summary>
	/// �F���擾
	/// </summary>
	/// <returns>�F</returns>
	inline XMFLOAT4& GetColor() { return m_color; }

	/// <summary>
	/// �F���Z�b�g
	/// </summary>
	/// <param name="color">�F</param>
	inline void SetColor(const XMFLOAT4& color)
	{
		m_color = color;
		m_dirty = true;
	}

	/// <summary>
	/// �^�C�����O�̎擾
	/// </summary>
	/// <returns>�^�C�����O</returns>
	inline XMFLOAT2& GetTiling() { return m_tiling; }

	/// <summary>
	/// �^�C�����O�̃Z�b�g
	/// </summary>
	/// <param name="tiling">�^�C�����O</param>
	inline void SetTiling(XMFLOAT2 tiling)
	{
		m_tiling = tiling;
	}
		
	/// <summary>
	/// �I�t�Z�b�g�̎擾
	/// </summary>
	/// <returns>�I�t�Z�b�g</returns>
	inline XMFLOAT2& GetOffset() { return m_offset; }

	/// <summary>
	/// �I�t�Z�b�g�̃Z�b�g
	/// </summary>
	/// <param name="offset">�I�t�Z�b�g</param>
	inline void SetOffset(XMFLOAT2 offset)
	{
		m_offset = offset;
	}

	/// <summary>
	/// ���f���f�[�^�Z�b�g
	/// </summary>
	inline void SetModel(Model* model)
	{
		m_model = model;
	}

	/// <summary>
	/// �r���{�[�h�Z�b�g
	/// </summary>
	inline void SetBillboard(const bool isBillboard)
	{
		m_isBillboard = isBillboard;
	}

	/// <summary>
	/// ���[���h�s����擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	inline const XMMATRIX& GetMatWorld() { return m_matWorld; }

	/// <summary>
	/// �y�A�����g�̃��[���h�s����擾
	/// </summary>
	/// <param name="parent">�y�A�����g�̃��[���h�s��</param>
	inline void SetParent(Object3d& parent)
	{ 
		m_parent.reset(&parent);
	}

	/// <summary>
	/// �R���C�_�[�̃Z�b�g
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo& info);
};