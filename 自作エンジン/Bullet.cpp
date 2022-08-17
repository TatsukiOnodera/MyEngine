#include "Bullet.h"

using namespace DirectX;

Bullet::Bullet(Object3d* bullet)
{
	m_object.reset(bullet);
	Initialize();
}

Bullet::~Bullet()
{

}

void Bullet::Initialize()
{
	m_vec = { 0, 0, 0 };
	m_alive = false;
}

void Bullet::Update()
{
	if (m_alive)
	{
		XMFLOAT3 bPos = m_object->GetPosition();
		bPos.x += m_vec.x;
		bPos.y += m_vec.y;
		bPos.z += m_vec.z;
		m_object->SetPosition(bPos);

		if (bPos.x < -300 || 300 < bPos.x)
		{
			m_alive = false;
		}
		else if (bPos.z < -300 || 300 < bPos.z)
		{
			m_alive = false;
		}

		m_object->Update();
	}
}

void Bullet::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (m_alive)
	{
		m_object->Draw(cmdList);
	}
}

void Bullet::SetPosition(XMFLOAT3 pos)
{
	m_object->SetPosition(pos);
}

void Bullet::SetVector(XMFLOAT3 vec)
{
	m_vec = vec;
}

void Bullet::SetAlive(bool alive)
{
	m_alive = alive;
}
