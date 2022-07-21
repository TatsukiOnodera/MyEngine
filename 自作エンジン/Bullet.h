#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>

#include "Object3d.h"

class Bullet
{
private: //�G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private: //�����o�ϐ�
	//�I�u�W�F�N�g�N���X
	std::unique_ptr< Object3d> m_bullet = nullptr;
	//���W
	XMFLOAT3 m_pos = {};
	//�x�N�g��
	XMFLOAT3 m_vec = {};
	//�����t���O
	bool m_alive = false;

public: //�����o�֐�
	//�R���X�g���N�^
	Bullet(XMFLOAT3 pos, XMFLOAT3 vec);

	//�f�X�g���N�^
	~Bullet();

	//������
	void Initialize(XMFLOAT3 pos, XMFLOAT3 vec);

	//�X�V
	void Update();

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//�����t���O���擾
	bool GetAlive() { return m_alive; }

	//�����t���O���Z�b�g
	void activeBullet(XMFLOAT3 pos, XMFLOAT3 vec);
};
