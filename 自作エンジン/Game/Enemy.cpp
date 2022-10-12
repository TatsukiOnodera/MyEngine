#include "Enemy.h"

Enemy::Enemy()
{
	m_object.reset(Object3d::Create("Dragon", true));
	Initialize();
}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	// null�`�F�b�N
	if (m_object == nullptr)
	{
		assert(0);
	}

	// ���W
	m_pos = { 0, 0, 50 };
	// ���x
	m_vel = { static_cast<float>(rand() % 21 - 10) / 10, -0.98f, static_cast<float>(rand() % 21 - 10) / 10 };
	// �����t���O
	m_alive = true;

	// �I�u�W�F�N�g
	m_object->SetPosition(m_pos);
	m_object->SetScale({ 3, 3, 3 });
	m_object->SetColor({ 0.0f, 0.3f, 0.9f, 1.0f });
	m_object->Update();
}

void Enemy::Update()
{
	if (m_alive == true)
	{
		//�x�N�g���̉��Z
		m_pos = m_object->GetPosition();

		m_pos.x += m_vel.x;
		m_pos.y += m_vel.y;
		m_pos.z += m_vel.z;

		m_object->SetPosition(m_pos);
	}
}

void Enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (m_alive == true)
	{
		// OBJ�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		m_object->Draw();

		Object3d::PostDraw();

		// FBX�I�u�W�F�N�g
		FbxObject3d::PreDraw(cmdList);



		FbxObject3d::PostDraw();
	}
}

void Enemy::OnCollision()
{
	m_alive = false;
}

void Enemy::SetPosition(const XMFLOAT3& position)
{
	m_pos = position;

	m_object->SetPosition(m_pos);
}

void Enemy::SetVelocity(const XMFLOAT3& velocity)
{
	m_vel = velocity;
}

void Enemy::SetAlive(const bool& alive)
{
	m_alive = alive;
}
