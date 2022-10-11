#include "Bullet.h"

using namespace DirectX;

Bullet::Bullet(XMFLOAT3 pos, XMFLOAT3 vec, bool alive)
{
	m_object.reset(Object3d::Create("Bullet", true));
	Initialize(pos, vec, alive);
}

Bullet::~Bullet()
{

}

void Bullet::Initialize(XMFLOAT3 pos, XMFLOAT3 vec, bool alive)
{
	if (m_object == nullptr)
	{
		assert(0);
	}

	m_pos = pos;
	m_vel = vec;
	m_alive = alive;

	m_object->SetPosition(m_pos);
	m_object->Update();
}

void Bullet::Update()
{
	if (m_alive == true)
	{
		//À•W‚ÌXV
		m_pos = m_object->GetPosition();

		m_pos.x += m_vel.x;
		m_pos.y += m_vel.y;
		m_pos.z += m_vel.z;

		m_object->SetPosition(m_pos);

		//•Ç‚Ì“–‚½‚è”»’è
		if (m_pos.x < -200 || 200 < m_pos.x)
		{
			m_alive = false;
		}
		else if (m_pos.z < -200 || 200 < m_pos.z)
		{
			m_alive = false;
		}
	}
}

void Bullet::Draw()
{
	if (m_alive == true)
	{
		m_object->Draw();
	}
}

void Bullet::SetPosition(XMFLOAT3 pos)
{
	m_pos = pos;
	m_object->SetPosition(m_pos);
}

void Bullet::SetVelocity(XMFLOAT3 vel)
{
	m_vel = vel;
}

void Bullet::SetAlive(bool alive)
{
	m_alive = alive;
}
