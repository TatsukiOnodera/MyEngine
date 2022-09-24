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
	if (m_object == nullptr)
	{
		assert(0);
	}

	m_pos = { 0, 0, 50 };
	m_vec = { 0, 0, 0 };
	m_alive = true;

	m_object->SetPosition(m_pos);
	m_object->SetScale({ 3, 3, 3 });
	m_object->SetColor({ 0.0f, 0.3f, 0.9f, 1.0f });
	m_object->Update();
}

void Enemy::Update()
{
	if (m_alive == true && m_pos.y == -100)
	{
		//ƒxƒNƒgƒ‹‚Ì‰ÁŽZ
		m_pos = m_object->GetPosition();

		m_pos.x += m_vec.x;
		m_pos.z += m_vec.z;

		m_object->SetPosition(m_pos);

		//•Ç‚Ì“–‚½‚è”»’è
		if (m_pos.x > 100)
		{
			m_vec.x = -fabs(static_cast<float>(rand() % 101 - 50) / 100);
		}
		else if (m_pos.x < -100)
		{
			m_vec.x = fabs(static_cast<float>(rand() % 101 - 50) / 100);
		}
		if (m_pos.z > 100)
		{
			m_vec.z = -fabs(static_cast<float>(rand() % 101 - 50) / 100);
		}
		else if (m_pos.z < -100)
		{
			m_vec.z = fabs(static_cast<float>(rand() % 101 - 50) / 100);
		}
	}
	else if (m_pos.y > -100)
	{
		m_pos = m_object->GetPosition();

		m_pos.y -= 0.98f;
		if (m_pos.y < -100)
		{
			m_pos.y = -100;
		}

		m_object->SetPosition(m_pos);
	}
}

void Enemy::Draw()
{
	if (m_alive == true)
	{
		m_object->Draw();
	}
}

const float Enemy::Length(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	XMFLOAT3 len = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}
