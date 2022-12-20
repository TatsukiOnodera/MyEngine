#include "AutoLockOn.h"
#include <SafeDelete.h>

AutoLockOn::AutoLockOn()
{

}

AutoLockOn::~AutoLockOn()
{

}

void AutoLockOn::Update()
{
	if (m_enemysList.empty() == true)
	{
		m_targetEnemy = nullptr;
		m_targetNum = 0;
	}
	else
	{
		if (m_targetEnemy == nullptr)
		{
			m_targetEnemy = m_enemysList.front();
		}
	}
}

void AutoLockOn::ChangeTargetLock()
{
	m_targetNum += 1;

	if (m_enemysList.empty() == false)
	{
		int tmp_count = 0;
		if (m_enemysList.size() < m_targetNum)
		{
			m_targetNum = 1;
		}
		else if (m_targetNum < 1)
		{
			m_targetNum = static_cast<int>(m_enemysList.size());
		}
		for (auto& m : m_enemysList)
		{
			tmp_count++;
			if (m == m_targetEnemy)
			{
				continue;
			}
			if (tmp_count == m_targetNum)
			{
				m_targetEnemy = m;
			}
		}
	}
	else
	{
		m_targetNum = 0;
	}
}

void AutoLockOn::LockOn(Enemy* enemy)
{
	if (InList(enemy) == false)
	{
		m_enemysList.emplace_back(enemy);

		if (m_targetEnemy == nullptr)
		{
			m_targetEnemy = enemy;
		}
		if (m_targetNum == 0)
		{
			m_targetNum = 1;
		}
	}
}

void AutoLockOn::Lost(Enemy * enemy)
{
	m_enemysList.remove(enemy);
	if (m_targetEnemy == enemy)
	{
		m_targetEnemy = nullptr;
	}
}

bool AutoLockOn::InList(Enemy * enemy)
{
	for (auto& m : m_enemysList)
	{
		if (enemy == m)
		{
			return true;
		}
	}

	return false;
}
