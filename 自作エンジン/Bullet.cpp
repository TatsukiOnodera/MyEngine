#include "Bullet.h"

using namespace DirectX;

Bullet::Bullet(XMFLOAT3 pos, XMFLOAT3 vec)
{
	m_bullet.reset(Object3d::Create("Bullet", true));
	m_bullet->SetScale({ 0.5, 0.5, 0.5 });
	Initialize(pos, vec);
}

Bullet::~Bullet()
{

}

void Bullet::Initialize(XMFLOAT3 pos, XMFLOAT3 vec)
{
	m_pos = pos;
	m_vec = vec;
	m_alive = true;
}

void Bullet::Update()
{
	if (m_alive)
	{
		m_pos = m_bullet->GetPosition();

		m_pos.x += m_vec.x;
		m_pos.y += m_vec.y;
		m_pos.z += m_vec.z;

		m_bullet->SetPosition(m_pos);

		if ((m_pos.x < -250 || 250 < m_pos.x) || (m_pos.z < -250 || 250 < m_pos.z))
		{
			m_alive = false;
		}
	}
}

void Bullet::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (m_alive)
	{
		m_bullet->Draw(cmdList);
	}
}

void Bullet::activeBullet(XMFLOAT3 pos, XMFLOAT3 vec)
{
	Initialize(pos, vec);
}
