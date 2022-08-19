#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Object3d.h"
#include "Bullet.h"

using namespace std;

class Enemy
{
public: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	/// <summary>
	/// ����
	/// </summary>
	/// <returns>Enemy</returns>
	static Enemy* Create();

private: // �����o�ϐ�
	//Enemy
	// �I�u�W�F�N�g
	unique_ptr<Object3d> m_object = nullptr;
	// ���W
	XMFLOAT3 m_pos = { 0, 0, 50 };
	// �ړ��x�N�g��
	XMFLOAT3 m_vec = { 0, 0, 0 };
	// �����t���O
	bool m_alive = false;
	//�V���b�g�̊Ԋu
	int intervalTime = 0;

	//Bullet
	vector<unique_ptr<Bullet>> bullet;

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
	/// <param name="playerPosition">���@�̍��W</param>
	void Update(XMFLOAT3 playerPosition);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���ׂĒe���g���Ă��邩
	/// </summary>
	/// <returns>����</returns>
	bool UsingAllBullet();

	/// <summary>
	/// �n�_����I�_�ւ̋���
	/// </summary>
	/// <param name="pos1">�I�_</param>
	/// <param name="pos2">�n�_</param>
	/// <returns>��_�Ԃ̋���</returns>
	const float Length(XMFLOAT3 pos1, XMFLOAT3 pos2);

	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	XMFLOAT3 GetPosition() { return m_pos; }
};
