#include "Enemy.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include <Tool.h>

Enemy::Enemy()
{
	// モデルの読み込み
	m_enemyOBJ.reset(Object3d::Create(2));

	// 初期化
	Initialize();
}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	// nullチェック
	if (m_enemyOBJ == nullptr)
	{
		assert(0);
	}

	// 座標
	m_pos = { static_cast<float>(rand() % 1501 - 750), 10, static_cast<float>(rand() % 1501) };
	// 速度
	m_vel = { static_cast<float>(rand() % 11 - 5) / 50, 0, static_cast<float>(rand() %11 - 5) / 50 };
	// HP
	m_HP = 3;
	// 生存フラグ
	m_alive = true;
	// 発射間隔
	m_bulletInterval = 0;
	// 重力加速値の時間
	m_gravityTime = 0;

	// オブジェクト
	m_enemyOBJ->SetPosition(m_pos);
	m_enemyOBJ->SetScale({ 2, 2, 2 });
	SphereCollider* collider = new SphereCollider({ 0, 0, 0 }, 2);
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	m_enemyOBJ->SetCollider(collider);
	m_enemyOBJ->Update();
}

void Enemy::Update()
{
	// エネミーの更新
	if (m_alive == true)
	{
		XMFLOAT3 acc = {};
		acc.y += (-0.98f * 0.25f) * powf(static_cast<float>(m_gravityTime) / 60, 2);

		// 重力時間
		m_gravityTime++;

		m_vel.x += acc.x;
		m_vel.y += acc.y;
		m_vel.z += acc.z;

		// 座標に速度を加算
		m_pos.x += m_vel.x;
		m_pos.y += m_vel.y;
		m_pos.z += m_vel.z;

		m_enemyOBJ->SetPosition(m_pos);

		// 弾の間隔
		m_bulletInterval++;
		if (90 < m_bulletInterval)
		{
			m_bulletInterval = 90;
		}

		// 当たり判定
		CheckCollision();
	}

	// 弾の更新
	for (auto& m : enemyBullets)
	{
		m->Update();
	}
}

void Enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// OBJオブジェクト描画
	Object3d::PreDraw(cmdList);
	
	if (m_alive == true)
	{
		m_enemyOBJ->Draw();
	}
	if (0 < enemyBullets.size())
	{
		for (auto& m : enemyBullets)
		{
			m->GetObject3d()->SetScale({ 2, 2, 2 });
			m->Draw();
		}
	}

	Object3d::PostDraw();
}

void Enemy::ShotBullet(const XMFLOAT3& targetPos)
{
	if (90 <= m_bulletInterval)
	{
		m_bulletInterval = 0;

		XMFLOAT3 vel = {};
		// 標的の座標を取得
		vel.x = targetPos.x - m_pos.x;
		vel.y = targetPos.y - m_pos.y;
		vel.z = targetPos.z - m_pos.z;

		// 長さを1にして10倍する
		float len = sqrtf(powf(vel.x, 2) + powf(vel.y, 2) + powf(vel.z, 2));
		vel.x = vel.x / len * 8.0f;
		vel.y = vel.y / len * 8.0f;
		vel.z = vel.z / len * 8.0f;

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
		// 使っていないのがあるか
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

bool Enemy::SearchTarget(const XMFLOAT3& target)
{
	// 範囲内なら
	if (Tool::LengthFloat3(m_pos, target) < 300.0f)
	{
		return true;
	}

	return false;
}

bool Enemy::EnemyDamage(int num)
{
	m_HP -= num;
	if (m_HP <= 0)
	{
		m_HP = 0;
		return true;
	}

	return false;
}

void Enemy::CheckCollision()
{
	SphereCollider* collider = dynamic_cast<SphereCollider*>(m_enemyOBJ->GetCollider());

	m_enemyOBJ->Update();

	Ray ray;
	ray.start = collider->center;
	ray.start.m128_f32[1] += collider->GetRadius();
	ray.dir = { 0, -1, 0, 0 };

	RayCastHit rayHit;
	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &rayHit, collider->GetRadius() * 2.0f + 0.2f))
	{
		m_pos.y -= (rayHit.m_distance - collider->GetRadius() * 2);
		m_vel.y = 0;
		m_enemyOBJ->SetPosition(m_pos);
		m_gravityTime = 0;
	}
}
