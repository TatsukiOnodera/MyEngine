#include "Enemy.h"

Enemy::Enemy(Object3d* enemy)
{
	m_object.reset(enemy);
	Initialize();
}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	assert(m_object);
	m_object->SetPosition({ 0, 0, 100 });
	m_object->SetScale({ 3, 3, 3 });
	m_object->SetColor({ 0, 0.3, 0.9, 1 });
	m_object->Update();

	m_vec = { static_cast<float>(rand() % 201 - 100) / 100, 0, static_cast<float>(rand() % 201 - 100) / 100 };

	m_alive = true;
}

void Enemy::Update(XMFLOAT3 playerPosition)
{
	if (m_alive)
	{
		//ベクトルの加算
		XMFLOAT3 ePos = m_object->GetPosition();
		ePos.x += m_vec.x;
		ePos.z += m_vec.z;
		m_object->SetPosition(ePos);

		//自機との距離
		if (30 > Length(ePos, playerPosition))
		{
			if (ePos.x - playerPosition.x > 0)
			{
				m_vec.x = fabs(static_cast<float>(rand() % 201 - 100) / 100);
			} 
			else if (ePos.x - playerPosition.x < 0)
			{
				m_vec.x = -fabs(static_cast<float>(rand() % 201 - 100) / 100);
			}
			if (ePos.z - playerPosition.z > 0)
			{
				m_vec.z = fabs(static_cast<float>(rand() % 201 - 100) / 100);
			} 
			else if (ePos.z - playerPosition.z < 0)
			{
				m_vec.z = -fabs(static_cast<float>(rand() % 201 - 100) / 100);
			}
		}

		//壁の当たり判定
		if (ePos.x > 100)
		{
			m_vec.x = -fabs(static_cast<float>(rand() % 201 - 100) / 100);
		}
		else if (ePos.x < -100)
		{
			m_vec.x = fabs(static_cast<float>(rand() % 201 - 100) / 100);
		}
		if (ePos.z > 100)
		{
			m_vec.z = -fabs(static_cast<float>(rand() % 201 - 100) / 100);
		} 
		else if (ePos.z < -100)
		{
			m_vec.z = fabs(static_cast<float>(rand() % 201 - 100) / 100);
		}
	}
}

void Enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (m_alive)
	{
		m_object->Draw(cmdList);
	}
}

const float Enemy::Length(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	XMFLOAT3 len = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}
