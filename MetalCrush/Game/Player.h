#pragma once
#include "Input.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"

#include "Bullet.h"

#include <DirectXMath.h>
#include <memory>

class Player
{
public: // �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �f�o�C�X
	// �J����
	static Camera* s_camera;
	// ����n
	static Input* s_input;

private: // �萔
	// �ʏ�ړ�
	// �����x
	const float c_accMove = 0.0325f;
	// �����x
	const float c_decMove = 0.9f;
	// �ő呬�x
	const float c_maxVelXZ = 0.6f;

	// �W�����v
	// �����x
	const float c_accJump = 0.05f;
	// �ő呬�x
	const float c_maxVelY = 0.6f;

	// �J����
	// ��]�p�x
	const float c_addAngle = 1.0f;

private: // �T�u�N���X
	// �v���C���[�̏��
	struct PlayerInfo
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
	std::unique_ptr<FbxObject3d> m_playerFBX = nullptr;
	// �v���C���[���
	PlayerInfo m_player;
	// �_�b�V���̉����x
	float m_dashAcc = 0;
	// �_�b�V���̎���
	float m_dashTime = 0;
	// 1�t���[��������̉��Z�iDT = DashTime�j
	float m_addDT = 10.0f;
	// �d�͉����l�̎���
	int m_gravityTime = 0;

	//==============================
	// �v���C���[�̒e
	//==============================
	// �I�u�W�F�N�g
	std::vector<std::unique_ptr<Bullet>> m_playerBullets;
	// ���f��
	Model* m_bulletModel = nullptr;
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
	// �p�[�e�B�N��
	//==============================
	std::unique_ptr<ParticleManager> m_booster;
	XMFLOAT3 m_mBoosterPos[2] = {};
	XMFLOAT3 m_sBoosterPos[2] = {};
	XMFLOAT3 m_bBoosterPos[2] = {};

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Player(Model* playerModel, Model* bulletModel);

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

	void UpdateOffset();

	/// <summary>
	/// �w�ʃu�[�X�^�[
	/// </summary>
	void MainBooster(const XMFLOAT3& acc);

	/// <summary>
	/// ���u�[�X�^�[
	/// </summary>
	void SideBooster(const XMFLOAT3& acc);

	/// <summary>
	/// �O�u�[�X�^�[
	/// </summary>
	void BacktBooster(const XMFLOAT3& acc);

	void DashBooster(const XMFLOAT3& acc);

	/// <summary>
	/// �n�_����I�_�ւ̋���
	/// </summary>
	/// <param name="pos1">�I�_</param>
	/// <param name="pos2">�n�_</param>
	/// <returns>��_�Ԃ̋���</returns>
	const float Length(const XMFLOAT3& pos1, const XMFLOAT3& pos2 = { 0, 0, 0 });

	/// <summary>
	/// ���K��
	/// </summary>
	/// <param name="tmp">���K������l</param>
	/// <returns>���K�������l</returns>
	const XMFLOAT3 normalize(const XMFLOAT3& tmp);

public: // �A�N�Z�b�T
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	inline const XMFLOAT3& GetPosition() { return m_player.pos; }
	
	/// <summary>
	/// void SetPosition(XMFLOAT3 position);
	/// </summary>
	/// <param name="position">���W</param>
	inline void SetPosition(const XMFLOAT3& position)
	{
		m_player.pos = position;

		m_playerFBX->SetPosition(m_player.pos);
	}

	/// <summary>
	/// ���x���擾
	/// </summary>
	/// <returns>���x</returns>
	inline const XMFLOAT3& GetVelocity() { return m_player.vel; }

	/// <summary>
	/// �����t���O���擾
	/// </summary>
	inline bool GetAlive() { return m_player.isAlive; }

	/// <summary>
	/// �����t���O���Z�b�g
	/// </summary>
	/// <param name="alive">�����t���O</param>
	inline void SetAlive(const bool alive)
	{
		m_player.isAlive = alive;
	}

	/// <summary>
	/// �W�I�̍��W���Z�b�g
	/// </summary>
	/// <param name="targetPosition">�W�I�̍��W</param>
	inline void SetTargetPosition(const XMFLOAT3& targetPosition)
	{
		m_targetPos = targetPosition;
	}

	/// <summary>
	/// ���b�N�t���O���Z�b�g
	/// </summary>
	inline void SetIsLock(const bool isLock)
	{
		m_isLock = isLock;
	}

	/// <summary>
	/// HP�̎擾
	/// </summary>
	/// <returns>HP</returns>
	inline int GetPlayerHP() { return m_player.HP; }

	/// <summary>
	/// HP�̃Z�b�g
	/// </summary>
	/// <param name="m_HP">HP</param>
	inline void SetPlayerHP(const int HP)
	{
		m_player.HP = HP;
	}

	/// <summary>
	/// ���e���擾
	/// </summary>
	/// <returns>���e��</returns>
	inline int GetBulletCapacity() { return m_bulletCapacity; }

	/// <summary>
	/// �_�b�V���t���O���擾
	/// </summary>
	inline bool GetIsDash() { return m_player.isDash; }

	/// <summary>
	/// FBX�擾
	/// </summary>
	inline FbxObject3d* GetPlayerObject() { return m_playerFBX.get(); }

	/// <summary>
	/// �e���擾
	/// </summary>
	inline const std::vector<std::unique_ptr<Bullet>>& GetPlayerBullets() { return m_playerBullets; }
};
