#include "Bullet.h"

using namespace DirectX;

Bullet::Bullet()
{

}

Bullet::~Bullet()
{

}

Bullet* Bullet::Create(XMFLOAT3 pos, XMFLOAT3 vec, bool alive)
{
	Bullet* bullet = new Bullet;

	bullet->Initialize(pos, vec, alive);

	return bullet;
}

void Bullet::Initialize(XMFLOAT3 pos, XMFLOAT3 vec, bool alive)
{
	if (m_object == nullptr)
	{
		m_object.reset(Object3d::Create("Bullet", true));
	}

	m_pos = pos;
	m_vec = vec;
	m_alive = alive;

	assert(m_object);
	m_object->SetPosition(m_pos);
	m_object->Update();
}

void Bullet::Update()
{
	if (m_alive)
	{
		//À•W‚ÌXV
		m_pos = m_object->GetPosition();
		m_pos.x += m_vec.x;
		m_pos.y += m_vec.y;
		m_pos.z += m_vec.z;
		m_object->SetPosition(m_pos);

		//•Ç‚Ì“–‚½‚è”»’è
		if (m_pos.x < -100 || 100 < m_pos.x)
		{
			m_alive = false;
		}
		else if (m_pos.z < -100 || 100 < m_pos.z)
		{
			m_alive = false;
		}
	}
}

void Bullet::Draw()
{
	if (m_alive)
	{
		m_object->Draw();
	}
}

void Bullet::SetPosition(XMFLOAT3 pos)
{
	m_pos = pos;
}

void Bullet::SetVector(XMFLOAT3 vec)
{
	m_vec = vec;
}

void Bullet::SetAlive(bool alive)
{
	m_alive = alive;
}
