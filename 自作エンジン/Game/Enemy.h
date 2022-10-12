#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Object3d.h"
#include "Player.h"
#include "Bullet.h"

class Enemy
{
public: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �����o�ϐ�
	//Enemy
	// �I�u�W�F�N�g
	std::unique_ptr<Object3d> m_object = nullptr;
	// ���W
	XMFLOAT3 m_pos = { 0, 0, 50 };
	// ���x
	XMFLOAT3 m_vel = { 0, 0, 0 };
	// �����t���O
	bool m_alive = false;

	//==============================
	// �G�l�~�[�̒e
	//==============================
	// �I�u�W�F�N�g
	std::vector<std::unique_ptr<Bullet>> enemyBullets;
	// �W�I�̍��W
	XMFLOAT3 m_targetPos = { 0, 0, 0 };

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Enemy();

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
	/// �Փˎ��̃R�[���o�b�N�֐�
	/// </summary>
	void OnCollision();

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
	void SetPosition(const XMFLOAT3& position);

	/// <summary>
	/// ���x���擾
	/// </summary>
	/// <returns>���x</returns>
	XMFLOAT3 GetVelocity() { return m_vel; }

	/// <summary>
	/// ���x���Z�b�g
	/// </summary>
	/// <param name="velocity">���x</param>
	void SetVelocity(const XMFLOAT3& velocity);

	/// <summary>
	/// �����t���O�̎擾
	/// </summary>
	/// <returns>�����t���O</returns>
	bool GetAlive() { return m_alive; }

	/// <summary>
	/// �����t���O�̃Z�b�g
	/// </summary>
	/// <param name="alive">����</param>
	void SetAlive(const bool& alive);

	/// <summary>
	/// �e���擾
	/// </summary>
	const std::vector<std::unique_ptr<Bullet>>& GetEnemyBullet() { return enemyBullets; }
};
