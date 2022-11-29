#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Player.h"
#include "Bullet.h"

#include <DirectXMath.h>
#include <memory>

class Enemy
{
public: // �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �����o�ϐ�
	//Enemy
	// �I�u�W�F�N�g
	std::unique_ptr<Object3d> m_object = nullptr;
	// ���W
	XMFLOAT3 m_pos = { 0, 0, 0 };
	// ���x
	XMFLOAT3 m_vel = { 0, 0, 0 };
	// �����t���O
	bool m_alive = false;

	//==============================
	// �G�l�~�[�̒e
	//==============================
	// �I�u�W�F�N�g
	std::vector<std::unique_ptr<Bullet>> enemyBullets;
	// ���f��
	Model* m_bulletModel = nullptr;
	// ���ˊԊu
	int m_bulletInterval = 0;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Enemy(Model* enemyModel, Model* bulletModel);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Enemy();

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
	/// �e�𔭎�
	/// </summary>
	/// <param name="targetPosition">�W�I�̍��W</param>
	void ShotBullet(const XMFLOAT3& targetPos);

	/// <summary>
	/// �g���Ă��Ȃ��e�����邩
	/// </summary>
	/// <returns>��</returns>
	bool CheckNoUsingBullet();

public: //�A�N�Z�b�T
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	XMFLOAT3 GetPosition() { return m_pos; }

	/// <summary>
	/// ���W���Z�b�g
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
	XMFLOAT3 GetVelocity() { return m_vel; }

	/// <summary>
	/// ���x���Z�b�g
	/// </summary>
	/// <param name="velocity">���x</param>
	void SetVelocity(const XMFLOAT3& velocity)
	{
		m_vel = velocity;
	}


	/// <summary>
	/// �����t���O�̎擾
	/// </summary>
	/// <returns>�����t���O</returns>
	bool GetAlive() { return m_alive; }

	/// <summary>
	/// �����t���O�̃Z�b�g
	/// </summary>
	/// <param name="alive">����</param>
	void SetAlive(const bool alive)
	{
		m_alive = alive;
	}

	/// <summary>
	/// �I�u�W�F�N�g�����擾
	/// </summary>
	/// <returns>�I�u�W�F�N�g</returns>
	Object3d* GetObject3d() { return m_object.get(); }

	/// <summary>
	/// �e���擾
	/// </summary>
	const std::vector<std::unique_ptr<Bullet>>& GetEnemyBullet() { return enemyBullets; }
};