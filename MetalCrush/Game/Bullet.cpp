#include "Bullet.h"

using namespace DirectX;

Bullet::Bullet(const XMFLOAT3& pos, const XMFLOAT3& vel, const bool& alive, Model* model)
{
	// nullチェック
	assert(model);

	m_object.reset(Object3d::Create(model));
	Initialize(pos, vel, alive);
}

Bullet::~Bullet()
{

}

void Bullet::Initialize(const XMFLOAT3& pos, const XMFLOAT3& vel, const bool& alive)
{
	if (m_object == nullptr)
	{
		assert(0);
	}

	m_pos = pos;
	m_vel = vel;
	m_alive = alive;

	m_object->SetPosition(m_pos);
	m_object->SetScale({ 0.5f, 0.5f, 0.5f });
	m_object->SetColor({ 0.9f, 0.3f, 0.1f, 1.0f });
	m_object->Update();
}

void Bullet::Update()
{
	if (m_alive == true)
	{
		// 座標に速度を加算
		m_pos.x += m_vel.x;
		m_pos.y += m_vel.y;
		m_pos.z += m_vel.z;

		m_object->SetPosition(m_pos);
	}
}

void Bullet::Draw()
{
	if (m_alive == true)
	{
		m_object->Draw();
	}
}
