#include "Enemy.h"
#include "SphereCollider.h"

Enemy::Enemy(Model* enemyModel, Model* bulletModel)
{
	// モデルの読み込み
	m_enemyOBJ.reset(Object3d::Create(enemyModel));
	m_bulletModel = bulletModel;

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
	m_pos = { static_cast<float>(rand() % 1001 - 500), 100, 100 };
	// 速度
	m_vel = { static_cast<float>(rand() % 5 - 2) / 10, -9.8f, static_cast<float>(rand() % 5 - 2) / 10 };
	//m_vel = { 0, -9.8f, 0 };
	// 生存フラグ
	m_alive = true;
	// 発射間隔
	m_bulletInterval = 0;

	// オブジェクト
	m_enemyOBJ->SetPosition(m_pos);
	m_enemyOBJ->SetScale({ 3, 3, 3 });
	m_enemyOBJ->SetColor({ 1.0f, 0.1f, 0.1f, 1.0f });
	m_enemyOBJ->SetCollider(new SphereCollider({ 0, 0, 0 }, 3));
	m_enemyOBJ->Update();
}

void Enemy::Update()
{
	// エネミーの更新
	if (m_alive == true)
	{
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
		vel.x = vel.x / len * 2.0f;
		vel.y = vel.y / len * 2.0f;
		vel.z = vel.z / len * 2.0f;

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
			enemyBullets.emplace_back(new Bullet(m_pos, vel, true, m_bulletModel));
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
