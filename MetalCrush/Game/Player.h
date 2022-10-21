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

private: // �T�u�N���X
	// �v���C���[�̃X�e�[�^�X
	struct PlayerStatus
	{
		// HP
		int HP = 20;

		// �����t���O
		bool isAlive = false;
		// �_�b�V���t���O
		bool isDash = false;

		// ���W
		XMFLOAT3 pos = { 0, 0, 0 };
		// ���x
		XMFLOAT3 vel = { 0, 0, 0 };
	};

private: // �����o�ϐ�
	//==============================
	// ���@
	//==============================
	// �I�u�W�F�N�g
	unique_ptr<FbxObject3d> m_object = nullptr;
	// �X�e�[�^�X
	PlayerStatus m_status;

	// �ʏ�ړ��̉����x
	const float c_accMove = 0.0325f;
	// �ʏ�ړ��̌����x
	const float c_decMove = 0.9f;
	// �ʏ�ړ��̍ő呬�x
	const float c_maxVelXZ = 0.6f;
	// �W�����v�̉����x
	const float c_accJump = 0.05f;
	// �W�����v�̍ő呬�x
	const float c_maxVelY = 0.6f;

	// �_�b�V���̉����x
	float m_dashAcc = 0;
	// �_�b�V���̎���
	float m_dashTime = 0;
	// 1�t���[��������̉��Z�iDT = DashTime�j
	float m_addDT = 6.0f;
	// �d�͉����l�̎���
	int m_gravityTime = 0;

	//==============================
	// �v���C���[�̒e
	//==============================
	// �I�u�W�F�N�g
	std::vector<std::unique_ptr<Bullet>> playerBullets;
	// �W�I�̍��W
	XMFLOAT3 m_targetPos = { 0, 0, 0 };
	// ���b�N����
	bool m_isLock = false;
	// ���ˊԊu
	int m_shotInterval = 0;
	// ���e��
	int m_bulletCapacity = 0;
	// �����[�h�^�C��
	int m_reloadTimer = 0;

	//==============================
	// �J����
	//==============================
	// �J�����̉�]�p�x
	const float c_addAngle = 0;
	// �J�����ʒu�̐��ʉ�
	bool m_cameraInitialize = false;

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
	void CameraWork();

	/// <summary>
	/// �n�ʂɒ��n������
	/// </summary>
	void OnLand();

public: // �A�N�Z�b�T
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3 GetPosition() { return m_status.pos; }
	
	/// <summary>
	/// void SetPosition(XMFLOAT3 position);
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const XMFLOAT3& position)
	{
		m_status.pos = position;

		m_object->SetPosition(m_status.pos);
	}

	/// <summary>
	/// ���x���擾
	/// </summary>
	/// <returns>���x</returns>
	const XMFLOAT3 GetVelocity() { return m_status.vel; }

	/// <summary>
	/// �����t���O���擾
	/// </summary>
	bool GetAlive() { return m_status.isAlive; }

	/// <summary>
	/// �����t���O���Z�b�g
	/// </summary>
	/// <param name="alive">�����t���O</param>
	void SetAlive(const bool alive)
	{
		m_status.isAlive = alive;
	}

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
	/// HP�̎擾
	/// </summary>
	/// <returns>HP</returns>
	int GetPlayerHP() { return m_status.HP; }

	/// <summary>
	/// HP�̃Z�b�g
	/// </summary>
	/// <param name="m_HP">HP</param>
	void SetPlayerHP(const int HP)
	{
		m_status.HP = HP;
	}

	/// <summary>
	/// ���e���擾
	/// </summary>
	/// <returns>���e��</returns>
	int GetBulletCapacity() { return m_bulletCapacity; }

	/// <summary>
	/// �_�b�V���t���O���擾
	/// </summary>
	bool GetIsDash() { return m_status.isDash; }

	/// <summary>
	/// �e���擾
	/// </summary>
	const std::vector<std::unique_ptr<Bullet>>& GetPlayerBullets() { return playerBullets; }
};
