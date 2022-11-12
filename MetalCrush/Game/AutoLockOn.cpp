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
	if (enemysList.empty() == true)
	{
		targetEnemy = nullptr;
		targetNum = 0;
	}
	else
	{
		if (targetEnemy == nullptr)
		{
			targetEnemy = enemysList.front();
		}
	}
}

void AutoLockOn::ChangeTargetLock()
{
	targetNum += 1;

	if (enemysList.empty() == false)
	{
		int tmp_count = 0;
		if (enemysList.size() < targetNum)
		{
			targetNum = 1;
		}
		else if (targetNum < 1)
		{
			targetNum = static_cast<int>(enemysList.size());
		}
		for (auto& m : enemysList)
		{
			tmp_count++;
			if (m == targetEnemy)
			{
				continue;
			}
			if (tmp_count == targetNum)
			{
				targetEnemy = m;
			}
		}
	}
	else
	{
		targetNum = 0;
	}
}

void AutoLockOn::LockOn(Enemy* enemy)
{
	if (InList(enemy) == false)
	{
		enemysList.emplace_back(enemy);

		if (targetEnemy == nullptr)
		{
			targetEnemy = enemy;
		}
		if (targetNum == 0)
		{
			targetNum = 1;
		}
	}
}

void AutoLockOn::Lost(Enemy * enemy)
{
	enemysList.remove(enemy);
	if (targetEnemy == enemy)
	{
		targetEnemy = nullptr;
	}
}

bool AutoLockOn::InList(Enemy * enemy)
{
	for (auto& m : enemysList)
	{
		if (enemy == m)
		{
			return true;
		}
	}

	return false;
}
