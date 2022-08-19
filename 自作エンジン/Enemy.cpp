#include "Enemy.h"

Enemy::Enemy()
{
	
}

Enemy::~Enemy()
{

}

Enemy* Enemy::Create()
{
	Enemy* enemy = new Enemy;

	enemy->Initialize();

	return enemy;
}

void Enemy::Initialize()
{
	m_object.reset(Object3d::Create("Dragon", true));

	m_vec = { static_cast<float>(rand() % 101 - 50) / 100, 0, static_cast<float>(rand() % 101 - 50) / 100 };

	m_alive = true;

	assert(m_object);
	m_object->SetPosition(m_pos);
	m_object->SetScale({ 3, 3, 3 });
	m_object->SetColor({ 0, 0.3, 0.9, 1 });
	m_object->Update();
}

void Enemy::Update(XMFLOAT3 playerPosition)
{
	if (m_alive)
	{
		//ベクトルの加算
		m_pos = m_object->GetPosition();
		m_pos.x += m_vec.x;
		m_pos.z += m_vec.z;
		m_object->SetPosition(m_pos);

		//自機との距離
		if (60 > Length(m_pos, playerPosition))
		{
			if (m_pos.x - playerPosition.x > 0)
			{
				m_vec.x = fabs(static_cast<float>(rand() % 101 - 50) / 100);
			} 
			else if (m_pos.x - playerPosition.x < 0)
			{
				m_vec.x = -fabs(static_cast<float>(rand() % 101 - 50) / 100);
			}
			if (m_pos.z - playerPosition.z > 0)
			{
				m_vec.z = fabs(static_cast<float>(rand() % 101 - 50) / 100);
			} 
			else if (m_pos.z - playerPosition.z < 0)
			{
				m_vec.z = -fabs(static_cast<float>(rand() % 101 - 50) / 100);
			}
		}

		//壁の当たり判定
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

		//弾の発射
		intervalTime++;
		if (intervalTime > 120)
		{
			intervalTime = 0;
			//すべて使われているか
			if (UsingAllBullet())
			{
				bullet.emplace_back(Bullet::Create(m_pos, { 0, 0, -1 }, true));
			} 
			else
			{
				for (auto& m : bullet)
				{
					if (m->GetAlive() == false)
					{
						m->Initialize(m_pos, { 0, 0, -1 }, true);
						break;
					}
				}
			}
		}

		//弾の更新
		for (auto& m : bullet)
		{
			m->Update();
		}
	}
}

void Enemy::Draw()
{
	if (m_alive)
	{
		m_object->Draw();
	}
	for (const auto& m : bullet)
	{
		m->Draw();
	}
}

bool Enemy::UsingAllBullet()
{
	for (const auto& m : bullet)
	{
		if (m->GetAlive() == false)
		{
			return false;
		}
	}

	return true;
}

const float Enemy::Length(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	XMFLOAT3 len = { pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z };

	return sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
}
