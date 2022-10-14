#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Input.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"

#include "Bullet.h"

using namespace std;

class Player
{
public: // �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �ÓI�����o�ϐ�
	// �J����
	static Camera* s_camera;
	// ����n
	static Input* s_input;

private: // �����o�ϐ�
	//==============================
	// ���@
	//==============================
	// �I�u�W�F�N�g
	unique_ptr<FbxObject3d> m_object = nullptr;
	// �����t���O
	bool m_alive = false;
	// ���W
	XMFLOAT3 m_pos = { 0, 0, 0 };
	// ���x
	XMFLOAT3 m_vel = { 0, 0, 0 };
	// �������x
	float m_accSpeed = 0;
	// �����x
	float m_decSpeed = 0;
	// �W�����v�����x
	float m_accJump = 0;
	// �d�͉����l�̎���
	int m_gravityTime = 0;
	// �_�b�V���t���O
	bool m_isDash = false;
	// �_�b�V��������
	float m_dashTimes = 0;
	// �_�b�V��������̖����Z�l
	float m_accDashTimes = 0;
	// �J�����̊p�x
	XMFLOAT3 m_angle = { 360, 0, 0 };
	// �J�����̉�]�p�x
	float m_addAngle = 0;
	// �J�����ʒu�̐��ʉ�
	bool m_cameraInitialize = false;

	//==============================
	// �v���C���[�̒e
	//==============================
	// �I�u�W�F�N�g
	std::vector<std::unique_ptr<Bullet>> playerBullets;
	// �W�I�̍��W
	XMFLOAT3 m_targetPos = { 0, 0, 0 };
	// ���b�N����
	bool m_isLock = false;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Player();

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
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �v���C���[�̈ړ�
	/// </summary>
	/// <param name="acc">�����l</param>
	void MovePlayer(XMFLOAT3& acc);

	/// <summary>
	/// �v���C���[�̃W�����v
	/// </summary>
	/// <param name="acc">�����l</param>
	void JumpPlayer(XMFLOAT3& acc);

	/// <summary>
	/// ���x�ɉ����x�����Z
	/// </summary>
	/// <param name="acc">�����l</param>
	void AddAcceleration(const XMFLOAT3& acc);

	/// <summary>
	/// �v���C���[�̃_�b�V��
	/// </summary>
	void DashPlayer();

	/// <summary>
	/// �e������
	/// </summary>
	void ShotBullet();

	/// <summary>
	/// �g���Ă��Ȃ��e�����邩
	/// </summary>
	/// <returns>��</returns>
	bool CheckNoUsingBullet();

	/// <summary>
	/// �J�������[�N
	/// </summary>
	void MoveCamera();

	// <summary>
	/// �J�����̐��ʉ�
	/// </summary>
	bool CameraMoveFront(float anglex, float angleY);

	/// <summary>
	/// �n�ʂɒ��n������
	/// </summary>
	void isLanding();

public: // �A�N�Z�b�T
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3 GetPosition() { return m_pos; }
	
	/// <summary>
	/// void SetPosition(XMFLOAT3 position);
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const XMFLOAT3& position)
	{
		m_pos = position;

		m_object->SetPosition(m_pos);
	}

	/// <summary>
	/// ���x���擾
	/// </summary>
	/// <returns>���x</returns>
	const XMFLOAT3 GetVelocity() { return m_vel; }

	/// <summary>
	/// �����t���O���擾
	/// </summary>
	bool GetAlive() { return m_alive; }

	/// <summary>
	/// �W�I�̍��W���Z�b�g
	/// </summary>
	/// <param name="targetPosition">�W�I�̍��W</param>
	void SetTargetPosition(const XMFLOAT3& targetPosition)
	{
		m_targetPos = targetPosition;
	}

	/// <summary>
	/// ���b�N�t���O���Z�b�g
	/// </summary>
	void SetIsLock(const bool isLock)
	{
		m_isLock = isLock;
	}

	/// <summary>
	/// �e���擾
	/// </summary>
	const std::vector<std::unique_ptr<Bullet>>& GetPlayerBullets() { return playerBullets; }
};
