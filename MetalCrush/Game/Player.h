#pragma once
#include "Input.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"
#include "Bullet.h"

#include <DirectXMath.h>
#include <memory>

/// <summary>
/// �v���C���[
/// </summary>
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

private: // �T�u�N���X
	// �v���C���[�̏��
	struct PlayerInfo
	{
		// HP
		int HP = 0;
		// �u�[�X�g�Q�[�W
		int boostGauge = 0;
		// �����t���O
		bool isAlive = false;
		// �_�b�V���t���O
		bool isDash = false;
		// ���W
		XMFLOAT3 pos = { 0, 0, 0 };
		// ���x
		XMFLOAT3 vel = { 0, 0, 0 };
	};

	//�ʏ� �ړ��p�����[�^
	struct MoveParameter
	{
		// �����x
		float acc = 0;
		// �����x
		float dec = 0;
		// �ő呬�x
		float maxVelXZ = 0;
	};

	// �_�b�V���p�����[�^
	struct DashParameter
	{
		// �_�b�V���̉����x
		float acc = 0;
		// �_�b�V���̎���
		float time = 0;
		// 1�t���[��������̉��Z�iDT = DashTime�j
		float DT = 0;
	};

	// �W�����v�p�����[�^�[
	struct JumpParameter
	{
		// �����x
		float acc = 0;
		// �ő�㏸���x
		float maxVelY = 0;
	};

	// �u�[�X�^�[�I�t�Z�b�g
	struct BoosterOffset
	{
		// ���C���u�[�X�^�[
		XMFLOAT3 mainOffset[2] = {};
		XMFLOAT3 main[2] = {};
		// �T�C�h�u�[�X�^�[
		XMFLOAT3 sideOffset[2] = {};
		XMFLOAT3 side[2] = {};
		// �o�b�N�u�[�X�^�[
		XMFLOAT3 backOffset[2] = {};
		XMFLOAT3 back[2] = {};
	};

	struct BoosterGaugeParameter
	{
		// �Q�[�W�ő�l
		int max = 0;
		// �񕜗�
		int add = 0;
		//	�u�[�X�g�N�[���^�C��
		int coolTime = 0;
		// �W�����v����
		int jumpDec = 0;
		// �_�b�V������
		int dashDec = 0;
	};

	// �o���b�g�p�����[�^�[
	struct BulletParameter
	{
		// �W�I�̍��W
		XMFLOAT3 targetPos = { 0, 0, 0 };
		// �ő呕�U��
		int maxNum = 0;
		// ���e��
		int num = 0;
		// �e��
		float speed = 0;
		// ���ˊԊu
		int interval = 0;
		// �����[�h�^�C��
		int reloadTimer = 0;
		// �����[�h����
		bool isReload = false;
		// ���b�N����
		bool isLock = false;
	};

	// �J�����p�����[�^
	struct CameraParameter
	{
		//	�����␳�t���O
		bool isMove = false;
		// ��]�p�x
		float addAngle = 0;
	};

private: // �����o�ϐ�
	//==============================
	// ���@
	//==============================
	// �I�u�W�F�N�g
	std::unique_ptr<Object3d> m_playerLeg = nullptr;
	std::unique_ptr<Object3d> m_playerBody = nullptr;
	std::unique_ptr<Object3d> m_playerArm = nullptr;
	// �v���C���[���
	PlayerInfo m_player;
	// �ʏ�ړ�
	MoveParameter m_move;
	// �_�b�V��
	DashParameter m_dash;
	// �W�����v
	JumpParameter m_jump;
	// �u�[�X�g�Q�[�W
	BoosterGaugeParameter m_gauge;
	// �d�͉����l�̎���
	int m_gravityTime = 0;

	//==============================
	// �v���C���[�̒e
	//==============================
	// �I�u�W�F�N�g
	std::vector<std::unique_ptr<Bullet>> m_playerBullets;
	// ���f��
	Model* m_bulletModel = nullptr;
	// �o���b�g
	BulletParameter m_bullet;

	//==============================
	// �p�[�e�B�N��
	//==============================
	// �I�u�W�F�N�g
	std::unique_ptr<ParticleManager> m_booster;
	// �I�t�Z�b�g
	BoosterOffset m_offset;

	//==============================
	// �J����
	//==============================
	CameraParameter m_camera;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Player(Model* bulletModel);

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

public: // �����o�֐�
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
	/// ���x�̐��K��
	/// </summary>
	/// <param name="acc">�����l</param>
	void NormalizeVel(const XMFLOAT3& acc);

	/// <summary>
	/// �v���C���[�̃_�b�V��
	/// </summary>
	void DashPlayer();

	/// <summary>
	/// �e������
	/// </summary>
	void ShotBullet();

	/// <summary>
	/// �J�������[�N
	/// </summary>
	void CameraWork();

	/// <summary>
	/// �I�t�Z�b�g�̍X�V
	/// </summary>
	void UpdateOffset();

	/// <summary>
	/// ���C���u�[�X�^�[
	/// </summary>
	void MainBooster();

	/// <summary>
	/// �T�C�h�u�[�X�^�[
	/// </summary>
	void SideBooster();

	/// <summary>
	/// �O�u�[�X�^�[
	/// </summary>
	void BacktBooster();

	/// <summary>
	/// �_�b�V�����̃u�[�X�^�[
	/// </summary>
	void DashBooster(const XMFLOAT3& acc);

	/// <summary>
	/// �u�[�X�^�[�̃Q�[�W
	/// </summary>
	void BoosterGauge();

	/// <summary>
	/// �n�ʂɒ��n������
	/// </summary>
	inline void HitGround()
	{
		m_gravityTime = 0;
		m_player.vel.y = 0;
	}

	/// <summary>
	/// �˒��ɂ��邩
	/// </summary>
	/// <param name="target">�Ώۂ̍��W</param>
	bool SearchTarget(const XMFLOAT3& traget);

	/// <summary>
	/// �^�[�Q�b�g��ݒ�
	/// </summary>
	/// <param name="pos">�^�[�Q�b�g�̍��W</param>
	/// <param name="isTraget">�^�[�Q�b�e�B���O���邩</param>
	inline void SetTarget(bool isTraget, const XMFLOAT3& pos = { 0, 0, 0 })
	{
		m_bullet.isLock = isTraget;
		m_bullet.targetPos = pos;
	}

public: // �����o�֐�
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

		m_playerLeg->SetPosition(m_player.pos);
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
	/// HP�̎擾
	/// </summary>
	/// <returns>HP</returns>
	inline int GetPlayerHP() { return m_player.HP; }

	/// <summary>
	/// �_���[�W�����炤
	/// </summary>
	/// <param name="num">�_���[�W�l</param>
	inline void PlayerDamage(int num)
	{
		m_player.HP -= num;
		if (m_player.HP < 0)
		{
			m_player.HP = 0;
		}
	}

	/// <summary>
	/// �u�[�X�g�Q�[�W�̎擾
	/// </summary>
	/// <returns>�u�[�X�g�Q�[�W</returns>
	inline int GetBoosterGauge() { return m_player.boostGauge; }

	/// <summary>
	/// ���e���擾
	/// </summary>
	/// <returns>���e��</returns>
	inline int GetBulletNum() { return m_bullet.num; }

	/// <summary>
	/// �_�b�V���t���O���擾
	/// </summary>
	inline bool GetIsDash() { return m_player.isDash; }

	/// <summary>
	/// FBX�擾
	/// </summary>
	inline Object3d* GetPlayerObject() { return m_playerLeg.get(); }

	/// <summary>
	/// �v���C���[�o���b�g���擾
	/// </summary>
	inline const std::vector<std::unique_ptr<Bullet>>& GetPlayerBullets() { return m_playerBullets; }
};
