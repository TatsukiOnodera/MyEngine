#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Object3d.h"

class Bullet
{
public: // �G�C���A�X
   // DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //�����o�ϐ�
	//�I�u�W�F�N�g
	std::unique_ptr<Object3d> m_object = nullptr;
	//�ړ��x�N�g��
	XMFLOAT3 m_vec = { 0, 0, 0 };
	//�����t���O
	bool m_alive = false;

public: //�����o�֐�
	//�R���X�g���N�^
	Bullet(Object3d* bullet);

	//�f�X�g���N�^
	~Bullet();

	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//���W���擾
	XMFLOAT3 GetPosition() { return m_object->GetPosition(); }

	//���W���Z�b�g
	void SetPosition(XMFLOAT3 pos);

	//�ړ��x�N�g�����擾
	XMFLOAT3 GetVector() { return m_vec; }

	//�ړ��x�N�g�����Z�b�g
	void SetVector(XMFLOAT3 vec);

	//�����t���O���擾
	bool GetAlive() { return m_alive; }

	//�����t���O���Z�b�g
	void SetAlive(bool alive);
};
