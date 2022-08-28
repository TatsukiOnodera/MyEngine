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
	if (m_object == nullptr)
	{
		m_object.reset(Object3d::Create("Dragon", true));
	}

	m_vec = { static_cast<float>(rand() % 101 - 50) / 100, -0.98f, static_cast<float>(rand() % 101 - 50) / 100 };
	m_alive = true;
	intervalTime = (rand() % 4 + 1) * 30;
	effectTimer = 0;

	assert(m_object);
	m_object->SetPosition(m_pos);
	m_object->SetScale({ 3, 3, 3 });
	m_object->SetColor({ 0, 0.3, 0.9, 1 });
	m_object->Update();
}

bool Enemy::Update(XMFLOAT3 playerPosition)
{
	if (m_alive && m_pos.y == -100)
	{
		//ベクトルの加算
		m_pos = m_object->GetPosition();
		m_pos.x += m_vec.x;
		m_pos.z += m_vec.z;
		m_object->SetPosition(m_pos);

		//自機との距離
		if (30 > Length(m_pos, playerPosition))
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
		if (effectTimer == 0)
		{
			intervalTime++;
		}
		else
		{
			effectTimer++;
		}
		if (intervalTime > 120)
		{
			intervalTime = 0;
			float bulletSpeed = 2;
			//すべて使われているか
			if (UsingAllBullet())
			{
				XMFLOAT3 len = { playerPosition.x - m_pos.x, playerPosition.y - m_pos.y, playerPosition.z - m_pos.z };
				len.x /= Length(playerPosition, m_pos);
				len.y /= Length(playerPosition, m_pos);
				len.z /= Length(playerPosition, m_pos);
				len.x *= bulletSpeed;
				len.y *= bulletSpeed;
				len.z *= bulletSpeed;
				bullet.emplace_back(Bullet::Create(m_pos, len, true));
			} 
			else
			{
				for (auto& m : bullet)
				{
					if (m->GetAlive() == false)
					{
						XMFLOAT3 len = { playerPosition.x - m_pos.x, playerPosition.y - m_pos.y, playerPosition.z - m_pos.z };
						len.x /= Length(playerPosition, m_pos);
						len.y /= Length(playerPosition, m_pos);
						len.z /= Length(playerPosition, m_pos);
						len.x *= bulletSpeed;
						len.y *= bulletSpeed;
						len.z *= bulletSpeed;
						m->Initialize(m_pos, len, true);
						break;
					}
				}
			}
		}

		//弾の更新
		bool f = false;
		for (auto& m : bullet)
		{
			if (m->Update(playerPosition))
			{
				f = true;
			}
		}

		return f;
	}
	else if (m_pos.y > -100)
	{
		m_pos = m_object->GetPosition();
		m_pos.y += m_vec.y;
		if (m_pos.y < -100)
		{
			m_pos.y = -100;
		}
		m_object->SetPosition(m_pos);

		return false;
	}
	else
	{
		return false;
	}
}

void Enemy::Draw()
{
	if (m_alive && effectTimer % 5 == 0)
	{
		if (effectTimer > 60)
		{
			Initialize();
			m_alive = false;
		}
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

void Enemy::SetEffectTimer()
{
	effectTimer = 1;
}
