#pragma once
#include "Enemy.h"

#include <assert.h>
#include <DirectXMath.h>
#include <list>
#include <memory>

class AutoLockOn
{
private: // �G�C���A�X
	using XMFLOAT3 = DirectX::XMFLOAT3;

private: // �����o�ϐ�
	// ���b�N�I�������Ώۂ̍��W���L�^
	std::list<Enemy*> m_enemysList;
	// �����b�N�I�����Ă�����W
	Enemy* m_targetEnemy = nullptr;
	// �W�I�̔ԍ�
	int m_targetNum = 0;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	AutoLockOn();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~AutoLockOn();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �Ώۂ̐؂�ւ�
	/// </summary>
	/// <param name="num">�G�l�~�[�̔ԍ�</param>
	void ChangeTargetLock();

	/// <summary>
	/// ���b�N�I��������
	/// </summary>
	void LockOn(Enemy * enemy);

	/// <summary>
	/// ���b�N�I�����O�ꂽ��
	/// </summary>
	void Lost(Enemy * enemy);

	/// <summary>
	/// ���X�g�ɂ��邩
	/// </summary>
	/// <param name="enemy">�Ώۂ̃G�l�~�[</param>
	bool InList(Enemy * enemy);

	/// <summary>
	/// �Ώۂ̃G�l�~�[���擾
	/// </summary>
	/// <returns>�Ώۂ̃G�l�~�[</returns>
	Enemy* GetTargetEnemy()
	{ 
		if (m_targetEnemy == nullptr)
		{
			assert(0);
		}

		return m_targetEnemy;
	}

	/// <summary>
	/// �W�I�̔ԍ��擾
	/// </summary>
	/// <returns>�W�I�̔ԍ�</returns>
	const int GetTargetNum() { return m_targetNum; }
};
