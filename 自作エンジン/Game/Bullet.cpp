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
	m_vec = vec;
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
	if (m_alive == true)
	{
		m_object->Draw();
	}
}

const float Bullet::Length(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	XMFLOAT3 len = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}

void Bullet::SetPosition(XMFLOAT3 pos)
{
	m_pos = pos;
	m_object->SetPosition(m_pos);
}

void Bullet::SetVector(XMFLOAT3 vec)
{
	m_vec = vec;
}

void Bullet::SetAlive(bool alive)
{
	m_alive = alive;
}
