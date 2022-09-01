#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Input.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "Bullet.h"
#include "Enemy.h"

using namespace std;

class Player
{
public: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private: //�ÓI�����o�ϐ�
	// ����n
	static Input* s_input;
	// �J����
	static Camera* s_camera;

private: // �����o�ϐ�
	// Player
	// �I�u�W�F�N�g
	unique_ptr<FbxObject3d> m_object = nullptr;
	// ���W
	XMFLOAT3 m_pos = { 0, 0, -50 };
	// �����t���O
	bool m_alive = false;
	// ��������
	bool m_isDash = false;
	// ���������l
	float m_add0 = 0;
	//�G�t�F�N�g�^�C�}�[
	int effectTimer = 0;

	// Bullet
	// �I�u�W�F�N�g
	vector<unique_ptr<Bullet>> bullet;

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
	void Draw();

	bool bulletUpdate(XMFLOAT3 enemyPos);

	void ShotBullet(XMFLOAT3 enemyPos);

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

public: // �A�N�Z�b�T
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	XMFLOAT3 GetPosition() { return m_pos; }

	void SetEffectTimer();
};
