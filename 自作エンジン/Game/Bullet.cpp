#include "Bullet.h"

using namespace DirectX;

Bullet::Bullet(const XMFLOAT3& pos, const XMFLOAT3& vel, const bool& alive)
{
	m_object.reset(Object3d::Create("Bullet", true));
	Initialize(pos, vel, alive);
}

Bullet::~Bullet()
{

}

void Bullet::Initialize(const XMFLOAT3& pos, const XMFLOAT3& vel, const bool& alive)
{
	if (m_object == nullptr)
	{
		assert(0);
	}

	m_pos = pos;
	m_vel = vel;
	m_alive = alive;
	m_targetPos = { 0, 0, 0 };

	m_object->SetPosition(m_pos);
	m_object->Update();
}

void Bullet::Update()
{
	if (m_alive == true)
	{
		//座標の更新
		m_pos = m_object->GetPosition();

		m_pos.x += m_vel.x;
		m_pos.y += m_vel.y;
		m_pos.z += m_vel.z;

		m_object->SetPosition(m_pos);
	}
}

void Bullet::Draw()
{
	if (m_alive == true)
	{
		m_object->Draw();
	}
}

void Bullet::OnCollision()
{
	m_alive = false;
}

void Bullet::SetPosition(const XMFLOAT3& pos)
{
	m_pos = pos;

	m_object->SetPosition(m_pos);
}

void Bullet::SetVelocity(const XMFLOAT3& vel)
{
	m_vel = vel;
}

void Bullet::SetAlive(const bool& alive)
{
	m_alive = alive;
}
