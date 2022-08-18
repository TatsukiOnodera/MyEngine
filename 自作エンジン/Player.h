#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Input.h"
#include "Camera.h"
#include "FbxObject3d.h"

class Player
{
public: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //�ÓI�����o�ϐ�
	// ����n
	static Input* s_input;
	// �J����
	static Camera* s_camera;

private: // �����o�ϐ�
	// �I�u�W�F�N�g
	std::unique_ptr<FbxObject3d> m_object = nullptr;
	// ���W
	XMFLOAT3 m_pos = {};
	// �����t���O
	bool m_alive = false;
	// ��������
	bool m_isDash = false;
	// ���������l
	float m_add0 = 0;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Player(FbxObject3d* player);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Player();

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
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	XMFLOAT3 GetPosition() { return m_pos; }
};
