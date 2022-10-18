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
	// null�`�F�b�N
	if (m_object == nullptr)
	{
		assert(0);
	}

	// ���W
	m_pos = { 0, 0, 50 };
	// ���x
	m_vel = { static_cast<float>(rand() % 21 - 10) / 10, -0.98f, static_cast<float>(rand() % 21 - 10) / 10 };
	// �����t���O
	m_alive = true;
	// ���ˊԊu
	m_bulletInterval = 60 * 2;

	// �I�u�W�F�N�g
	m_object->SetPosition(m_pos);
	m_object->SetScale({ 3, 3, 3 });
	m_object->SetColor({ 0.0f, 0.3f, 0.9f, 1.0f });
	m_object->Update();
}

void Enemy::Update()
{
	if (m_alive == true)
	{
		// �����x
		//XMFLOAT3 acc = {};

		// ���x�ɉ����x�����Z
		//m_vel.x += acc.x;
		//m_vel.y += acc.y;
		//m_vel.z += acc.z;

		// ���W�ɑ��x�����Z
		m_pos.x += m_vel.x;
		m_pos.y += m_vel.y;
		m_pos.z += m_vel.z;

		m_object->SetPosition(m_pos);

		// �e�̍X�V
		m_bulletInterval++;
		if (60 * 2 < m_bulletInterval)
		{
			m_bulletInterval = 60 * 2;
		}
		for (auto& m : enemyBullets)
		{
			m->Update();
		}
	}
}

void Enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// OBJ�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);
	
	if (m_alive == true)
	{
		m_object->Draw();
	}
	if (0 < enemyBullets.size())
	{
		for (auto& m : enemyBullets)
		{
			m->Draw();
		}
	}

	Object3d::PostDraw();
}

void Enemy::ShotBullet(const XMFLOAT3& targetPos)
{
	if (60 * 2 <= m_bulletInterval)
	{
		m_bulletInterval = 0;

		XMFLOAT3 vel = {};
		// �W�I�̍��W���擾
		vel.x = targetPos.x - m_pos.x;
		vel.y = targetPos.y - m_pos.y;
		vel.z = targetPos.z - m_pos.z;

		// ������1�ɂ���10�{����
		float len = sqrtf(powf(vel.x, 2) + powf(vel.y, 2) + powf(vel.z, 2));
		vel.x = vel.x / len * 5;
		vel.y = vel.y / len * 5;
		vel.z = vel.z / len * 5;

		if (CheckNoUsingBullet() == true)
		{
			for (auto& m : enemyBullets)
			{
				if (m->GetAlive() == false)
				{
					m->Initialize(m_pos, vel, true);
					break;
				}
			}
		}
		else
		{
			enemyBullets.emplace_back(new Bullet(m_pos, vel, true));
		}
	}
}

bool Enemy::CheckNoUsingBullet()
{
	if (0 < enemyBullets.size())
	{
		// �g���Ă��Ȃ��̂����邩
		bool hit = false;
		for (const auto& m : enemyBullets)
		{
			if (m->GetAlive() == false)
			{
				hit = true;
				break;
			}
		}

		return hit;
	}
	else
	{
		return false;
	}
}
