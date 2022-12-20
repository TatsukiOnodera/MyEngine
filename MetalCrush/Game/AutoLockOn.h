#pragma once
#include "Enemy.h"

#include <assert.h>
#include <DirectXMath.h>
#include <list>
#include <memory>

class AutoLockOn
{
private: // エイリアス
	using XMFLOAT3 = DirectX::XMFLOAT3;

private: // メンバ変数
	// ロックオンした対象の座標を記録
	std::list<Enemy*> m_enemysList;
	// 今ロックオンしている座標
	Enemy* m_targetEnemy = nullptr;
	// 標的の番号
	int m_targetNum = 0;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	AutoLockOn();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AutoLockOn();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 対象の切り替え
	/// </summary>
	/// <param name="num">エネミーの番号</param>
	void ChangeTargetLock();

	/// <summary>
	/// ロックオンしたら
	/// </summary>
	void LockOn(Enemy * enemy);

	/// <summary>
	/// ロックオンが外れたら
	/// </summary>
	void Lost(Enemy * enemy);

	/// <summary>
	/// リストにあるか
	/// </summary>
	/// <param name="enemy">対象のエネミー</param>
	bool InList(Enemy * enemy);

	/// <summary>
	/// 対象のエネミーを取得
	/// </summary>
	/// <returns>対象のエネミー</returns>
	Enemy* GetTargetEnemy()
	{ 
		if (m_targetEnemy == nullptr)
		{
			assert(0);
		}

		return m_targetEnemy;
	}

	/// <summary>
	/// 標的の番号取得
	/// </summary>
	/// <returns>標的の番号</returns>
	const int GetTargetNum() { return m_targetNum; }
};
