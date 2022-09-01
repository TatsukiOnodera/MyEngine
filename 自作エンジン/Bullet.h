#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Object3d.h"

using namespace std;

class Bullet
{
public: // �G�C���A�X
   // DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �����o�ϐ�
	// �I�u�W�F�N�g
	unique_ptr<Object3d> m_object = nullptr;
	// ���W
	XMFLOAT3 m_pos = { 0, 0, 0 };
	// �ړ��x�N�g��
	XMFLOAT3 m_vec = { 0, 0, 0 };
	// �����t���O
	bool m_alive = false;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Bullet(XMFLOAT3 pos, XMFLOAT3 vec, bool alive);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Bullet();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="vec">�x�N�g��</param>
	/// <param name="alive">�����t���O</param>
	void Initialize(XMFLOAT3 pos, XMFLOAT3 vec, bool alive);

	/// <summary>
	///  �X�V
	/// </summary>
	bool Update(XMFLOAT3 pos);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �n�_����I�_�ւ̋���
	/// </summary>
	/// <param name="pos1">�I�_</param>
	/// <param name="pos2">�n�_</param>
	/// <returns>��_�Ԃ̋���</returns>
	const float Length(XMFLOAT3 pos1, XMFLOAT3 pos2);

public: // �A�N�Z�b�T
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	XMFLOAT3 GetPosition() { return m_pos; }

	/// <summary>
	/// ���W���Z�b�g
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPosition(XMFLOAT3 pos);

	/// <summary>
	/// �ړ��x�N�g�����擾
	/// </summary>
	/// <returns>�x�N�g��</returns>
	XMFLOAT3 GetVector() { return m_vec; }

	/// <summary>
	/// �ړ��x�N�g�����Z�b�g
	/// </summary>
	/// <param name="vec">�x�N�g��</param>
	void SetVector(XMFLOAT3 vec);

	/// <summary>
	/// �����t���O���擾
	/// </summary>
	/// <returns>�����t���O</returns>
	bool GetAlive() { return m_alive; }

	/// <summary>
	/// �����t���O���Z�b�g
	/// </summary>
	/// <param name="alive">�����t���O</param>
	void SetAlive(bool alive);
};
